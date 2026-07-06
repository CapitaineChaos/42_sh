/* Table des opérateurs et redirections du lexer. La classe décide l'émission et
 * l'effet sur la pile de contexte :
 *   OPC_CONTROL   ;            séparateur, aucun effet de contexte
 *   OPC_OPERATOR  && || |      structure l'arbre, empile un contexte
 *   OPC_REDIR     > < >> <<    redirection, dépile le contexte
 *
 * Table ordonnée du plus long au plus court : un scan linéaire premier-match
 * réalise le maximal munch (|| avant |, >> avant >). */

#include "module_token.h"
#include "module_lexer.h"
#include "module_input.h"
#include "module_checker.h"

typedef enum e_op_class
{
	OPC_CONTROL,
	OPC_OPERATOR,
	OPC_REDIR,
}	t_op_class;

typedef struct s_op_spec
{
	char		*sym;
	t_tk_type	type;
	t_op_class	cls;
}	t_op_spec;

static const t_op_spec	g_ops[] = {
	{"&&", TOK_AND_IF, OPC_OPERATOR},
	{"||", TOK_OR_IF, OPC_OPERATOR},
	{">>", TOK_REDIR_APPEND, OPC_REDIR},
	{"<<", TOK_REDIR_HEREDOC, OPC_REDIR},
	{"|", TOK_PIPE, OPC_OPERATOR},
	{";", TOK_SEMI, OPC_CONTROL},
	{">", TOK_REDIR_OUT, OPC_REDIR},
	{"<", TOK_REDIR_IN, OPC_REDIR},
	{NULL, 0, 0},
};

/* Émet le token selon sa classe, dans l'ordre des effets. */
static void	emit_op(t_lexer *lx, const t_op_spec *op)
{
	if (op->cls == OPC_REDIR)
	{
		rem_op_context(lx);
		tk_word_emit(lx);
		tk_redir_emit(lx, op->sym, op->type);
	}
	else if (op->cls == OPC_OPERATOR)
	{
		tk_word_emit(lx);
		tk_operator_emit(lx, op->sym, op->type);
		add_context(lx, op->type);
	}
	else
	{
		tk_word_emit(lx);
		tk_control_emit(lx, op->sym, op->type);
	}
}

/* Scanne la famille demandée (redirections, ou contrôle/opérateurs) et consomme
 * le premier symbole qui correspond. */
static bool	scan_ops(t_lexer *lx, bool want_redir)
{
	int	i;

	for (i = 0; g_ops[i].sym; i++)
	{
		if (want_redir != (g_ops[i].cls == OPC_REDIR))
			continue ;
		if (match_str(&lx->inp, g_ops[i].sym))
		{
			emit_op(lx, &g_ops[i]);
			return (true);
		}
	}
	return (false);
}

bool	lex_operator(t_lexer *lx)
{
	return (scan_ops(lx, false));
}

bool	lex_redirection(t_lexer *lx)
{
	return (scan_ops(lx, true));
}
