/* Correspondance token -> nœud AST (type + classe). La précédence
 * (get_precedence) reste explicite : elle encode l'algèbre des opérateurs, pas
 * une simple correspondance. */

#include "module_parser.h"

typedef struct s_tok_ast
{
	t_tk_type	tok;
	t_ast_type	ast;
	t_ast_class	cls;
}	t_tok_ast;

static const t_tok_ast	g_tok_ast[] = {
	{TOK_PIPE, AST_PIPE, ACL_OPERATOR},
	{TOK_AND_IF, AST_AND, ACL_OPERATOR},
	{TOK_OR_IF, AST_OR, ACL_OPERATOR},
	{TOK_SEMI, AST_SEQ, ACL_CONTROL},
	{TOK_NEWLINE, AST_SEQ, ACL_CONTROL},
	{TOK_LPAREN, AST_SUBSH_MARK, ACL_OPERATOR},
	{TOK_RPAREN, AST_SUBSH_END, ACL_OPERAND},
	{TOK_WORD, AST_UNKNOWN, ACL_OPERAND},
};

t_ast_type	get_ast_type(t_tk_type type)
{
	size_t	i;

	for (i = 0; i < sizeof(g_tok_ast) / sizeof(g_tok_ast[0]); i++)
		if (g_tok_ast[i].tok == type)
			return (g_tok_ast[i].ast);
	return (AST_UNKNOWN);
}

t_ast_class	get_ast_class(t_tk_type type)
{
	size_t	i;

	for (i = 0; i < sizeof(g_tok_ast) / sizeof(g_tok_ast[0]); i++)
		if (g_tok_ast[i].tok == type)
			return (g_tok_ast[i].cls);
	return (ACL_OPERAND);
}

size_t	get_precedence(t_ast_type type)
{
	if (type == AST_PIPE)
		return (3);
	if (type == AST_AND || type == AST_OR)
		return (2);
	if (type == AST_SEQ)
		return (1);
	return (0);
}
