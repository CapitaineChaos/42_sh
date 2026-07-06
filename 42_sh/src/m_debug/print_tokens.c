/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:02:11 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_lexer.h"
#include "ft_std.h"
#include "utils.h"
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static char	ft_escape_code(unsigned char c)
{
	if (c == '\\' || c == '"')
		return (c);
	else if (c == '\n')
		return ('n');
	else if (c == '\r')
		return ('r');
	else if (c == '\t')
		return ('t');
	else if (c == '\a')
		return ('a');
	else if (c == '\b')
		return ('b');
	else if (c == '\f')
		return ('f');
	else if (c == '\v')
		return ('v');
	return (0);
}

void	debug_escape_print(char *s)
{
	size_t			i;
	unsigned char	c;
	static char		hex[] = "0123456789ABCDEF";
	char			e;
	t_logger		lg;

	log_init(&lg);
	i = 0;
	while (s && s[i])
	{
		c = (unsigned char)s[i++];
		if (c >= 32 && c <= 126 && c != '\\' && c != '"')
			log_putch(&lg, c);
		else
		{
			e = ft_escape_code(c);
			if (e)
			{
				log_putch(&lg, '\\');
				log_putch(&lg, e);
			}
			else
			{
				log_putch(&lg, 'x');
				log_putch(&lg, hex[c >> 4]);
				log_putch(&lg, hex[c & 0x0F]);
			}
		}
	}
	log_flush(-1, &lg, true);
}

void	print_astlist(t_ast_node *node)
{
	t_ast_node	*current;
	char		buf[100];

	if (!node)
		return ;
	current = node;
	if (current->tclass == ACL_OPERAND)
	{
		t_token	*token;

		token = current->t_ast_data.operand.tokens.head;
		while (token)
		{				
			sprintf(buf, "%s\n", debug_get_token_type(token->type));
			debug_log(buf);
			token = token->next;
		}
	}
	else
	{
		sprintf(buf, "%s\n", debug_get_token_type(current->t_ast_data.operator_.token->type));
		debug_log(buf);
	}
	print_astlist(current->left);
	print_astlist(current->right);
}
void	debug_simple_token_list_all_nodes(t_ast_node *node)
{
	if (DLVL < LVL_OK)
		return ;
	debug_title(LVL_OK, "DEBUG AST TOKENS");
	if (!node)
	{
		trace_short_info(LVL_OK, "AST is NULL or empty");
		return ;
	}
	print_astlist(node);
	debug_nano_title(LVL_OK, "End debug ast tokens");
}

static void	print_token_part(t_tk_part *part)
{
	char		*str;
	t_logger	lg;

	if (DLVL < LVL_OK)
		return ;
	str = part->stream;
	if (part)
	{
		if (str != NULL)
		{
			log_init(&lg);
			log_puts(&lg, "        -> Part Type: ");
			log_puts(&lg, debug_get_token_type(part->type));
			log_puts(&lg, " >>>     [");
			log_flush(-1, &lg, true);
			debug_escape_print(str);
			log_init(&lg);
			log_puts(&lg, "]\n");
			log_flush(-1, &lg, true);
		}
		else
		{
			log_init(&lg);
			log_puts(&lg, "        -> Part Type: ");
			log_puts(&lg, debug_get_token_type(part->type));
			log_puts(&lg, " >>>     [NULL]\n");
			log_flush(-1, &lg, true);
		}
	}
	else
	{
		log_init(&lg);
		log_puts(&lg, "         -> Word Part is NULL\n");
		log_flush(-1, &lg, true);
	}
}

static void	print_parts(t_tk_part *first)
{
	t_tk_part	*part;

	if (DLVL < LVL_OK)
		return ;
	if (!first)
	{
		trace_short_info(LVL_OK, "   -> Token has no parts");
		return ;
	}
	part = first;
	trace_short_info_no_nl(LVL_OK, "      -> Full content :                     [");
	while (part)
	{
		debug_escape_print(part->stream);
		part = part->next;
	}
	trace_short_info_no_nl(LVL_OK, "]\n");
	part = first;
	while (part)
	{
		print_token_part(part);
		part = part->next;
	}
}

void	print_token(t_token *token)
{
	t_logger	lg;

	if (DLVL < LVL_OK)
		return ;
	if (token)
	{
		log_init(&lg);
		log_puts(&lg, "          -> Token Type: ");
		log_puts(&lg, debug_get_token_type(token->type));
		log_puts(&lg, " \n");
		log_flush(-1, &lg, true);
		if (token->type == TOK_WORD || token->type == TOK_DELIM)
		{
			log_init(&lg);
			log_puts(&lg, "     -> Token parts: ");
			log_putlong(&lg, token->count);
			log_puts(&lg, "\n");
			log_flush(-1, &lg, true);
			if (token->head)
				print_parts(token->head);
		}
		else if (token->family == TKF_OPERATOR)
		{
			log_init(&lg);
			log_puts(&lg, "           -> Op Content:        ");
			log_puts(&lg, "           [");
			log_puts(&lg, token->str);
			log_puts(&lg, "]\n");
			log_flush(-1, &lg, true);
		}
		else if (token->type == TOK_HEREDOC_BODY)
		{
			log_init(&lg);
			log_puts(&lg, "           -> Heredoc Body:              ");
			log_puts(&lg, token->str ? token->str : "NULL");
			log_flush(-1, &lg, true);
		}
	}
	else
	{
		log_init(&lg);
		log_puts(&lg, "    -> Token is NULL\n");
		log_flush(-1, &lg, true);        
	}
}

void	debug_tokens(t_tokens *tokens, int lv)
{
	t_token		*current;
	size_t		i;
	t_logger	lg;

	log_init(&lg);
	if (lv < 3)
		return ;
	if (DLVL < LVL_OK)
		return ;
	debug_nano_title(LVL_OK, "DEBUG TOKENS LIST");
	if (!tokens || tokens->count == 0)
	{
		trace_short_info(LVL_OK, "   -> Token list is NULL or empty");
		return ;
	}
	trace_info_nvnb(LVL_OK, "Nb tokens", tokens->count);
	current = tokens->head;
	i = 0;
	while (current)
	{
		trace_info_nvnb(LVL_OK, "Index", i);
		print_token(current);
		current = current->next;
		i++;
	}
	debug_nano_title(LVL_OK, "End debug");
}



void	get_token_content_stream(t_token *token, char *buf, size_t buf_size)
{
	t_tk_part	*part;
	size_t		i;
	size_t		j;

	if (token == NULL || buf == NULL || buf_size < 5)
		return ;
	i = 0;
	part = token->head;
	while (part && i < buf_size - 1)
	{
		if (part->stream)
		{
			j = 0;
			while (part->stream[j] && i < buf_size - 1 && i < 70)
			{
				buf[i] = part->stream[j];
				i++;
				j++;
			}
			// Si on atteint 70 caractères, on ajoute "..." et on arrête
			if (i == 70)
			{
				if (buf_size > 73)
				{
					buf[70] = '.';
					buf[71] = '.';
					buf[72] = '.';
					buf[73] = '\0';
				}
				else
				{
					// Si le buffer n'a pas la place pour ..., on termine proprement
					if (buf_size - 1 > 0)
						buf[buf_size - 1] = '\0';
					if (buf_size > 3)
					{
						if (buf_size > 3)
							buf[buf_size - 4] = '.';
						if (buf_size > 2)
							buf[buf_size - 3] = '.';
						if (buf_size > 1)
							buf[buf_size - 2] = '.';
					}
				}
				return ;
			}
		}
		part = part->next;
	}
	buf[i] = '\0';
}

void	get_token_content_str(t_token *token, char *buf, size_t buf_size)
{
	t_tk_part	*part;
	size_t		i;
	size_t		j;

	if (token == NULL || buf == NULL || buf_size < 5)
		return ;
	i = 0;
	part = token->head;
	while (part && i < buf_size - 1)
	{
		if (part->str)
		{
			j = 0;
			while (part->str[j] && i < buf_size - 1 && i < 70)
			{
				buf[i] = part->str[j];
				i++;
				j++;
			}
			// Si on atteint 70 caractères, on ajoute "..." et on arrête
			if (i == 70)
			{
				if (buf_size > 73)
				{
					buf[70] = '.';
					buf[71] = '.';
					buf[72] = '.';
					buf[73] = '\0';
				}
				else
				{
					// Si le buffer n'a pas la place pour ..., on termine proprement
					if (buf_size - 1 > 0)
						buf[buf_size - 1] = '\0';
					if (buf_size > 3)
					{
						if (buf_size > 3)
							buf[buf_size - 4] = '.';
						if (buf_size > 2)
							buf[buf_size - 3] = '.';
						if (buf_size > 1)
							buf[buf_size - 2] = '.';
					}
				}
				return ;
			}
		}
		part = part->next;
	}
	buf[i] = '\0';
}

void	get_tkpart_content_str(t_tk_part *part, char *buf, size_t buf_size)
{
	size_t	i;
	size_t	j;

	if (part == NULL || buf == NULL || buf_size < 5)
		return ;
	i = 0;
	while (part && i < buf_size - 1)
	{
		if (part->str)
		{
			j = 0;
			while (part->str[j] && i < buf_size - 1 && i < 70)
			{
				buf[i] = part->str[j];
				i++;
				j++;
			}
			if (i == 70)
			{
				if (buf_size > 73)
				{
					buf[70] = '.';
					buf[71] = '.';
					buf[72] = '.';
					buf[73] = '\0';
				}
				else
				{
					if (buf_size - 1 > 0)
						buf[buf_size - 1] = '\0';
					if (buf_size > 3)
						buf[buf_size - 4] = '.';
					if (buf_size > 2)
						buf[buf_size - 3] = '.';
					if (buf_size > 1)
						buf[buf_size - 2] = '.';
				}
				return ;
			}
		}
		part = part->next;
	}
	buf[i] = '\0';
}

void	get_tkpart_content_stream(t_tk_part *part, char *buf, size_t buf_size)
{
	size_t	i;
	size_t	j;

	if (part == NULL || buf == NULL || buf_size < 5)
		return ;
	i = 0;
	while (part && i < buf_size - 1)
	{
		if (part->stream)
		{
			j = 0;
			while (part->stream[j] && i < buf_size - 1 && i < 70)
			{
				buf[i] = part->stream[j];
				i++;
				j++;
			}
			if (i == 70)
			{
				if (buf_size > 73)
				{
					buf[70] = '.';
					buf[71] = '.';
					buf[72] = '.';
					buf[73] = '\0';
				}
				else
				{
					if (buf_size - 1 > 0)
						buf[buf_size - 1] = '\0';
					if (buf_size > 3)
						buf[buf_size - 4] = '.';
					if (buf_size > 2)
						buf[buf_size - 3] = '.';
					if (buf_size > 1)
						buf[buf_size - 2] = '.';
				}
				return ;
			}
		}
		part = part->next;
	}
	buf[i] = '\0';
}

// Ajoute src à dest si place, toujours null-terminate, retourne la nouvelle longueur
size_t	safe_strcat(char *dest, const char *src, size_t bufsize)
{
	size_t	dest_len;
	size_t	i;

	dest_len = 0;
	while (dest[dest_len] && dest_len < bufsize)
		dest_len++;
	i = 0;
	while (src[i] && dest_len + i + 1 < bufsize)
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	if (dest_len + i < bufsize)
		dest[dest_len + i] = '\0';
	else if (bufsize > 0)
		dest[bufsize - 1] = '\0';
	return (dest_len + i);
}

void	print_token_kind_family(t_token *token, char *buf, size_t buf_size)
{

	buf[0] = '\0';
	safe_strcat(buf, debug_get_tok_kind(token->kind), buf_size);
	safe_strcat(buf, " / ", buf_size);
	safe_strcat(buf, get_tok_family(token->family), buf_size);
}


void	debug_tokens_list_str(t_token *list)
{
	char		buf[74];
	char		tmp[74];
	char		typebuf[512];
	t_tk_part	*part;
	int			idx;
	int			prefix_len;
	int			space_left;
	char		titre[64];

	ft_bzero(titre, 64);
	snprintf(titre, 64, "LISTE TOKENS STR (%zd parts)", list->count);
	idx = 0;
	while (list)
	{
		ft_bzero(buf, 74);
		get_tkpart_content_str(list->head, buf, 74);
		print_token_kind_family(list, typebuf, sizeof(typebuf));
		debug_mini_title(LVL_OK, titre);
		snprintf(typebuf + ft_strlen(typebuf), sizeof(typebuf) - ft_strlen(typebuf),
			"\n        quoted:%d  tilde:%d  dollar:%d  glob:%d\n",
			list->has_quoted_part, list->has_tilde, list->has_dollar, list->has_glob);
		debug_log(typebuf);

		/* format du prefixe */
		prefix_len = snprintf(tmp, sizeof(tmp), "        [%d] |", idx);
		if (prefix_len < 0)
			prefix_len = 0;
		space_left = sizeof(tmp) - prefix_len - 3; /* pour "|\\n" et \0 */

		if (space_left < 0)
			space_left = 0;
		/* copie du contenu de buf, tronqué si nécessaire */
		ft_strlcpy(tmp + prefix_len, buf, space_left + 1);
		safe_strcat(tmp, "|\n", sizeof(tmp));
		debug_log(tmp);

		part = list->head;
		while (part)
		{
			snprintf(buf, sizeof(buf), "        %-12s: str = |%s|\n",
				debug_get_token_type(part->type), part->str ? part->str : "");
			debug_log(buf);
			part = part->next;
		}
		list = list->next;
		idx++;
	}
	debug_mini_title(LVL_OK, "FIN LISTE TOKENS");
}


void	debug_tokens_list_stream(t_token *list)
{
	char		buf[74];
	char		tmp[74];
	char		typebuf[512];
	t_tk_part	*part;
	int			idx;
	int			prefix_len;
	int			space_left;
	char		titre[64];

	ft_bzero(titre, 64);
	snprintf(titre, 64, "LISTE TOKENS STREAM (%zd parts)", list->count);
	idx = 0;
	while (list)
	{
		ft_bzero(buf, 74);
		get_tkpart_content_str(list->head, buf, 74);
		print_token_kind_family(list, typebuf, sizeof(typebuf));
		debug_mini_title(LVL_OK, titre);
		snprintf(typebuf + ft_strlen(typebuf), sizeof(typebuf) - ft_strlen(typebuf),
			"\n        quoted:%d  tilde:%d  dollar:%d  glob:%d\n",
			list->has_quoted_part, list->has_tilde, list->has_dollar, list->has_glob);
		debug_log(typebuf);

		/* format du préfixe */
		prefix_len = snprintf(tmp, sizeof(tmp), "        [%d] |", idx);
		if (prefix_len < 0)
			prefix_len = 0;
		space_left = sizeof(tmp) - prefix_len - 3; /* pour "|\\n" et \0 */
		if (space_left < 0)
			space_left = 0;
		ft_strlcpy(tmp + prefix_len, buf, space_left + 1);
		safe_strcat(tmp, "|\n", sizeof(tmp));
		debug_log(tmp);

		part = list->head;
		while (part)
		{
			snprintf(buf, sizeof(buf), "        %-12s: str = |%s|\n",
				debug_get_token_type(part->type), part->stream ? part->stream : "");
			debug_log(buf);
			part = part->next;
		}
		list = list->next;
		idx++;
	}
	debug_mini_title(LVL_OK, "FIN LISTE TOKENS");
}
