/* Catégorie d'un token : prédicats dérivés du type. La classification n'est ni
 * stockée ni un enum ; le type est la seule source de vérité. `group`, lui, est
 * un état contextuel (position du token), posé à l'initialisation puis réaffecté
 * par le parser. */

#include "module_token.h"

bool	is_word(t_tk_type t)
{
	return (t == TOK_WORD || t == TOK_GLOB || t == TOK_DELIM);
}

bool	is_redirect(t_tk_type t)
{
	return (t == TOK_REDIR_IN || t == TOK_REDIR_OUT
		|| t == TOK_REDIR_APPEND || t == TOK_REDIR_HEREDOC);
}

bool	is_control(t_tk_type t)
{
	return (t == TOK_SEMI);
}

bool	is_operator(t_tk_type t)
{
	return (t == TOK_PIPE || t == TOK_AND_IF || t == TOK_OR_IF);
}

bool	is_heredoc_body(t_tk_type t)
{
	return (t == TOK_HEREDOC_BODY);
}

bool	is_subshell(t_tk_type t)
{
	return (t == TOK_LPAREN || t == TOK_RPAREN);
}

/* Opérande : ni opérateur ni parenthèse (mot, redirection, corps de heredoc). */
bool	is_operand(t_tk_type t)
{
	return (is_word(t) || is_redirect(t) || is_heredoc_body(t));
}

/* Connecteur : opérateur qui structure l'arbre en nœud binaire (| && || ;). */
bool	is_connector(t_tk_type t)
{
	return (is_operator(t) || is_control(t));
}

void	token_init_group(t_token *tok)
{
	if (is_redirect(tok->type) || is_heredoc_body(tok->type)
		|| is_connector(tok->type) || is_subshell(tok->type))
		tok->group = TKG_OTHER;
	else
		tok->group = TKG_ARGUMENT;
}
