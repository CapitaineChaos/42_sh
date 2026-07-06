/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dbg_view.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by smaitre           #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "all_config.h"
#include "module_debug.h"
#include "module_ast.h"
#include "module_token.h"
#include "ft_deque.h"
#include "ft_printf_fd.h"

/*
 * Writers pour le debugview web (tools/debugview). Chaque phase du pipeline
 * dépose un fichier dans DBG_VIEW_DIR ; debugserve.py les relit et le viewer
 * les rend en panneaux. Format (source de vérité : debugview.js) :
 *   lignes = '\n', champs = TAB, sous-listes = US (0x1f), sous-champs = RS (0x1e).
 * Le texte des mots vit dans part->stream (pré-expansion) ; op->tokens /
 * op->redirections persistent dans le parent (contrairement à op->argv, bâti
 * dans l'enfant forké). Actif uniquement en build debug (-DDBG_VIEW).
 */

#ifdef DBG_VIEW

# define DBG_VIEW_DIR "/dev/shm/sh42_dbg"
# define DBG_TAB '\t'
# define DBG_US '\x1f'
# define DBG_RS '\x1e'
# define DBG_NL '\n'

static int	open_file(const char *path)
{
	return (open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644));
}

static void	trunc_file(const char *path)
{
	int	fd;

	fd = open_file(path);
	if (fd >= 0)
		close(fd);
}

static void	put_byte(int fd, char c)
{
	write(fd, &c, 1);
}

/* Écrit s en neutralisant les octets de contrôle qui casseraient le format. */
static void	put_clean(int fd, const char *s)
{
	char	c;

	if (!s)
		return ;
	while (*s)
	{
		c = *s;
		if (c == DBG_TAB || c == DBG_NL || c == DBG_RS || c == DBG_US)
			c = ' ';
		write(fd, &c, 1);
		s++;
	}
}

/* Ignore le padding d'alignement des convertisseurs terminal. */
static const char	*ltrim(const char *s)
{
	while (*s == ' ')
		s++;
	return (s);
}

/* Texte d'un token : assemblé depuis parts (stream), repli sur str (opérateurs). */
static void	put_tok_text(int fd, t_token *t)
{
	char	buf[256];

	buf[0] = '\0';
	if (t->head)
		get_token_content_stream(t, buf, sizeof(buf));
	if (buf[0])
		put_clean(fd, buf);
	else if (t->str)
		put_clean(fd, t->str);
}

/* Liste de tokens en US-liste (argv de commande, redirections). */
static void	put_tok_list(int fd, t_tokens *toks)
{
	t_token	*t;
	int		first;

	t = toks->head;
	first = 1;
	while (t)
	{
		if (!first)
			put_byte(fd, DBG_US);
		first = 0;
		put_tok_text(fd, t);
		t = t->next;
	}
}

static void	put_part_flags(int fd, t_tk_part *p)
{
	put_byte(fd, p->has_dollar ? '$' : '-');
	put_byte(fd, p->has_tilde ? '~' : '-');
	put_byte(fd, p->has_glob ? '*' : '-');
}

/* parts : US-liste ; chaque part = type RS offset RS flags RS text. */
static void	put_parts(int fd, t_token *t)
{
	t_tk_part	*p;
	char		buf[256];
	int			first;

	p = t->head;
	first = 1;
	while (p)
	{
		if (!first)
			put_byte(fd, DBG_US);
		first = 0;
		ft_printf_fd(fd, "%s", debug_get_token_type(p->type));
		put_byte(fd, DBG_RS);
		ft_printf_fd(fd, "%d", (int)p->offset);
		put_byte(fd, DBG_RS);
		put_part_flags(fd, p);
		put_byte(fd, DBG_RS);
		buf[0] = '\0';
		get_tkpart_content_stream(p, buf, sizeof(buf));
		put_clean(fd, buf);
		p = p->next;
	}
}

static void	put_tok_flags(int fd, t_token *t)
{
	put_byte(fd, t->has_quoted_part ? 'q' : '-');
	put_byte(fd, t->has_dollar ? '$' : '-');
	put_byte(fd, t->has_tilde ? '~' : '-');
	put_byte(fd, t->has_glob ? '*' : '-');
}

static char	*ast_title(t_ast_node *n)
{
	if (n->type == AST_SEQ)
		return ("SEQ");
	return (get_ast_typestr(n->type));
}

static char	*ast_kind(t_ast_node *n)
{
	if (n->tclass == ACL_OPERAND)
		return ("operand");
	if (n->tclass == ACL_OPERATOR)
		return ("operator");
	if (n->tclass == ACL_STRUCT)
		return ("struct");
	return ("control");
}

/* Parcours préfixe : depth TAB kind TAB title TAB argv TAB redirs TAB meta. */
static void	ast_walk(int fd, t_ast_node *n, int depth)
{
	if (!n)
		return ;
	ft_printf_fd(fd, "%d\t%s\t%s\t", depth, ast_kind(n), ast_title(n));
	if (n->tclass == ACL_OPERAND)
		put_tok_list(fd, &n->t_ast_data.operand.tokens);
	put_byte(fd, DBG_TAB);
	if (n->tclass == ACL_OPERAND)
		put_tok_list(fd, &n->t_ast_data.operand.redirections);
	put_byte(fd, DBG_TAB);
	ft_printf_fd(fd, "id=%d", n->id);
	put_byte(fd, DBG_US);
	ft_printf_fd(fd, "arity=%d", n->arity);
	put_byte(fd, DBG_NL);
	ast_walk(fd, n->left, depth + 1);
	ast_walk(fd, n->right, depth + 1);
}

/* exec : seq cmd argc path exit status pid args redirs (une ligne/commande). */
static void	exec_walk(int fd, t_ast_node *n)
{
	t_operand	*op;

	if (!n)
		return ;
	if (n->tclass == ACL_OPERAND)
	{
		op = &n->t_ast_data.operand;
		ft_printf_fd(fd, "%d\t", n->id);
		if (op->tokens.head)
			put_tok_text(fd, op->tokens.head);
		ft_printf_fd(fd, "\t%d\t%s\t%d\t%d\t%d\t", (int)op->tokens.count,
			op->path ? op->path : "", n->exit_code, n->status, n->pid);
		put_tok_list(fd, &op->tokens);
		put_byte(fd, DBG_TAB);
		put_tok_list(fd, &op->redirections);
		put_byte(fd, DBG_NL);
	}
	exec_walk(fd, n->left);
	exec_walk(fd, n->right);
}

#endif

void	dbg_reset(void)
{
#ifdef DBG_VIEW
	if (mkdir(DBG_VIEW_DIR, 0755) < 0 && errno != EEXIST)
		return ;
	trunc_file(DBG_VIEW_DIR "/tokens");
	trunc_file(DBG_VIEW_DIR "/ast");
	trunc_file(DBG_VIEW_DIR "/exec");
	trunc_file(DBG_VIEW_DIR "/footer");
#endif
}

void	dbg_tokens(t_tokens *tokens)
{
#ifdef DBG_VIEW
	int		fd;
	int		idx;
	t_token	*t;

	fd = open_file(DBG_VIEW_DIR "/tokens");
	if (fd < 0)
		return ;
	idx = 0;
	t = tokens->head;
	while (t)
	{
		ft_printf_fd(fd, "%d\t%s\t%s\t", idx, ltrim(get_tok_family(t->family)),
			debug_get_token_type(t->type));
		put_tok_flags(fd, t);
		put_byte(fd, DBG_TAB);
		put_tok_text(fd, t);
		ft_printf_fd(fd, "\t%d\t", (int)t->count);
		put_parts(fd, t);
		put_byte(fd, DBG_NL);
		t = t->next;
		idx++;
	}
	close(fd);
#else
	(void)tokens;
#endif
}

void	dbg_ast(t_deque *final)
{
#ifdef DBG_VIEW
	int	fd;

	fd = open_file(DBG_VIEW_DIR "/ast");
	if (fd < 0)
		return ;
	ast_walk(fd, peek_head(final), 0);
	close(fd);
#else
	(void)final;
#endif
}

void	dbg_exec(t_ast_node *root)
{
#ifdef DBG_VIEW
	int	fd;

	fd = open_file(DBG_VIEW_DIR "/exec");
	if (fd < 0)
		return ;
	exec_walk(fd, root);
	close(fd);
#else
	(void)root;
#endif
}

void	dbg_footer(int last_exit)
{
#ifdef DBG_VIEW
	int	fd;

	fd = open_file(DBG_VIEW_DIR "/footer");
	if (fd < 0)
		return ;
	ft_printf_fd(fd, "exit=%d", last_exit);
	close(fd);
#else
	(void)last_exit;
#endif
}
