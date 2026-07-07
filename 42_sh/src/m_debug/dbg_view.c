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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "all_config.h"
#include "utils.h"
#include "module_debug.h"
#include "module_ast.h"
#include "module_token.h"
#include "ft_deque.h"

#ifdef DBG_VIEW

# define DBG_VIEW_DIR "/dev/shm/sh42_dbg"

# define DBG_F_READ DBG_VIEW_DIR "/00_read"
# define DBG_F_TOKENS DBG_VIEW_DIR "/01_tokens"
# define DBG_F_NODES DBG_VIEW_DIR "/02_nodes"
# define DBG_F_AST DBG_VIEW_DIR "/03_ast"
# define DBG_F_EXEC DBG_VIEW_DIR "/04_exec"
# define DBG_F_ERRORS DBG_VIEW_DIR "/05_errors"
# define DBG_F_FOOTER DBG_VIEW_DIR "/footer"
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

/* Texte d'un token : assemblé depuis parts, repli sur str (opérateurs). */
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

/* Émet une US-liste à partir d'un token de tête (argv, mots). */
static void	put_tok_seq(int fd, t_token *t)
{
	int	first;

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

/* Liste de tokens en US-liste (argv de commande, mots). */
static void	put_tok_list(int fd, t_tokens *toks)
{
	put_tok_seq(fd, toks->head);
}

static int	is_redir_op(t_token *t)
{
	return (t->type == TOK_REDIR_IN || t->type == TOK_REDIR_OUT
		|| t->type == TOK_REDIR_APPEND || t->type == TOK_REDIR_HEREDOC);
}

/*
 * Redirections groupées : chaque opérateur (>, >>, <, <<) est collé à sa cible
 * dans un même élément US ("> o"), pour ne pas les afficher comme deux redirs.
 */
static void	put_redir_list(int fd, t_tokens *toks)
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
		if (is_redir_op(t) && t->next)
		{
			put_byte(fd, ' ');
			t = t->next;
			put_tok_text(fd, t);
		}
		t = t->next;
	}
}

static void	put_part_flags(int fd, t_tk_part *p)
{
	put_byte(fd, p->has_dollar ? '$' : '-');
	put_byte(fd, p->has_tilde ? '~' : '-');
	put_byte(fd, p->has_glob ? '*' : '-');
}

/* parts : US-liste ; chaque part = part_type RS span RS flags RS text. */
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
		dprintf(fd, "%s", debug_get_part_type(p->type));
		put_byte(fd, DBG_RS);
		dprintf(fd, "%d:%d", (int)p->start, (int)p->end);
		put_byte(fd, DBG_RS);
		put_part_flags(fd, p);
		put_byte(fd, DBG_RS);
		buf[0] = '\0';
		get_tkpart_content_stream(t, p, buf, sizeof(buf));
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

/* Kind court attendu par nodesTable : "cmd" déclenche le split argv (US). */
static char	*node_kind_short(t_ast_node *n)
{
	if (n->tclass == ACL_OPERAND)
		return ("cmd");
	if (n->tclass == ACL_OPERATOR)
		return ("op");
	if (n->tclass == ACL_STRUCT)
		return ("struct");
	return ("ctrl");
}

static void	node_row(int fd, t_ast_node *n, int idx)
{
	dprintf(fd, "%d\t%s\t", idx, node_kind_short(n));
	if (n->tclass == ACL_OPERAND)
		put_tok_list(fd, &n->t_ast_data.operand.tokens);
	else
		dprintf(fd, "%s", ast_title(n));
	put_byte(fd, DBG_TAB);
	if (n->tclass == ACL_OPERAND)
		put_redir_list(fd, &n->t_ast_data.operand.redirections);
	put_byte(fd, DBG_TAB);
	dprintf(fd, "%s\t%d\t", ast_title(n), n->arity);
	if (n->tclass == ACL_OPERATOR)
		dprintf(fd, "%d", (int)n->t_ast_data.operator_.precedence);
	put_byte(fd, DBG_NL);
}

static void	nodes_walk(int fd, t_ast_node *n, int *idx)
{
	if (!n)
		return ;
	node_row(fd, n, (*idx)++);
	nodes_walk(fd, n->left, idx);
	nodes_walk(fd, n->right, idx);
}

/* Parcours préfixe : depth TAB kind TAB title TAB argv TAB redirs TAB meta. */
static void	ast_walk(int fd, t_ast_node *n, int depth)
{
	if (!n)
		return ;
	dprintf(fd, "%d\t%s\t%s\t", depth, ast_kind(n), ast_title(n));
	if (n->tclass == ACL_OPERAND)
		put_tok_list(fd, &n->t_ast_data.operand.tokens);
	put_byte(fd, DBG_TAB);
	if (n->tclass == ACL_OPERAND)
		put_redir_list(fd, &n->t_ast_data.operand.redirections);
	put_byte(fd, DBG_TAB);
	dprintf(fd, "id=%d", n->id);
	put_byte(fd, DBG_US);
	dprintf(fd, "arity=%d", n->arity);
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
		dprintf(fd, "%d\t", n->id);
		if (op->tokens.head)
			put_tok_text(fd, op->tokens.head);
		dprintf(fd, "\t%d\t%s\t%d\t%d\t%d\t", (int)op->tokens.count,
			op->path ? op->path : "", n->exit_code, n->status, n->pid);
		put_tok_list(fd, &op->tokens);
		put_byte(fd, DBG_TAB);
		put_redir_list(fd, &op->redirections);
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
	trunc_file(DBG_F_READ);
	trunc_file(DBG_F_TOKENS);
	trunc_file(DBG_F_NODES);
	trunc_file(DBG_F_AST);
	trunc_file(DBG_F_EXEC);
	trunc_file(DBG_F_FOOTER);
#endif
}

/*
 * Entrée brute reconstruite depuis data->inputs (chunks pré-lexer).
 * Rendu par le fallback <pre> du viewer.
 */
void	dbg_read(t_sstr *inputs)
{
#ifdef DBG_VIEW
	int		fd;
	char	*flat;

	fd = open_file(DBG_F_READ);
	if (fd < 0)
		return ;
	flat = sstrs_flatten(inputs);
	if (flat)
		dprintf(fd, "%s", flat);
	free(flat);
	close(fd);
#else
	(void)inputs;
#endif
}

void	dbg_tokens(t_tokens *tokens)
{
#ifdef DBG_VIEW
	int		fd;
	int		idx;
	t_token	*t;

	fd = open_file(DBG_F_TOKENS);
	if (fd < 0)
		return ;
	idx = 0;
	t = tokens->head;
	while (t)
	{
		dprintf(fd, "%d\t%s\t%s\t", idx,
			debug_get_token_role(t->role), debug_get_token_type(t->type));
		put_tok_flags(fd, t);
		put_byte(fd, DBG_TAB);
		put_tok_text(fd, t);
		dprintf(fd, "\t%d\t", (int)t->count);
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

void	dbg_nodes(t_ast_node *root)
{
#ifdef DBG_VIEW
	int	fd;
	int	idx;

	fd = open_file(DBG_F_NODES);
	if (fd < 0)
		return ;
	idx = 0;
	nodes_walk(fd, root, &idx);
	close(fd);
#else
	(void)root;
#endif
}

void	dbg_ast(t_deque *final)
{
#ifdef DBG_VIEW
	int	fd;

	fd = open_file(DBG_F_AST);
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

	fd = open_file(DBG_F_EXEC);
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

	fd = open_file(DBG_F_FOOTER);
	if (fd < 0)
		return ;
	dprintf(fd, "exit=%d", last_exit);
	close(fd);
#else
	(void)last_exit;
#endif
}

/*
 * Une erreur = un appel = une ligne (les octets de contrôle, dont les '\n'
 * internes des messages multi-lignes, sont neutralisés en espaces). Fichier
 * en append, remis à zéro par dbg_reset au début de chaque cycle de lecture.
 */
void	dbg_error(const char *msg)
{
#ifdef DBG_VIEW
	int	fd;

	if (!msg || !*msg)
		return ;
	fd = open(DBG_F_ERRORS, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return ;
	put_clean(fd, msg);
	put_byte(fd, DBG_NL);
	close(fd);
#else
	(void)msg;
#endif
}

/* Log de dev formaté vers le panel (macro DBG_LOGF). */
void	dbg_logf(const char *fmt, ...)
{
#ifdef DBG_VIEW
	char	buf[1024];
	va_list	ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	dbg_error(buf);
#else
	(void)fmt;
#endif
}

/* Assertion de dev échouée -> panel (macro DBG_ASSERT). */
void	dbg_assert_fail(const char *expr, const char *msg,
	const char *file, int line)
{
#ifdef DBG_VIEW
	char	buf[1024];

	snprintf(buf, sizeof(buf), "ASSERT(%s)%s%s @ %s:%d", expr,
		msg && *msg ? " — " : "", msg ? msg : "", file, line);
	dbg_error(buf);
#else
	(void)expr;
	(void)msg;
	(void)file;
	(void)line;
#endif
}

/* Remet le panel à zéro en début de session (les faults accumulent ensuite). */
void	dbg_errors_reset(void)
{
#ifdef DBG_VIEW
	if (mkdir(DBG_VIEW_DIR, 0755) < 0 && errno != EEXIST)
		return ;
	trunc_file(DBG_F_ERRORS);
#endif
}
