/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_input_string.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/14 16:21:32 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/14 16:21:32 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	error_with_cleanup(t_list **tokens)
{
	ft_lstclear(tokens, token_del);
	return (SYS_ERROR);
}

static int	init_cmd_nodes(t_shell *shell, t_list **cmd_node)
{
	t_list	*cmd;
	t_node	*node;

	node = node_new_def();
	cmd = ft_lstnew(node);
	if (!cmd || !cmd->content)
	{
		ft_lstdelone(cmd, node_del);
		return (SYS_ERROR);
	}
	shell->cmd_nodes = cmd;
	*cmd_node = NULL;
	return (1);
}

static int	reset_cmd_node(t_shell *shell, t_list **cmd_node)
{
	t_list	*cmd;
	t_node	*node;

	cmd = *cmd_node;
	ft_lstadd_back(&shell->cmd_nodes, cmd);
	node = node_new_def();
	*cmd_node = ft_lstnew(node);
	if (!(*cmd_node) || !(*cmd_node)->content)
	{
		ft_lstdelone((*cmd_node), node_del);
		return (SYS_ERROR);
	}
	return (1);
}

static void	move_and_unlink_token(t_list **tokens, t_list **node)
{
	t_list	*unlinked;

	*node = (*node)->next;
	unlinked = ft_lstunlink(tokens, *tokens);
	ft_lstdelone(unlinked, token_del);
}

static int	consume_token(t_list *cmd_node, t_token *token)
{

}

/*
** group_tokens() groups the t_token structures
** into a t_node structure for the executor.
** Redirects are put into the list t_node.redir.
** structures and the program, together with
** its arguments, is stored inside t_node.cmd.
**
** tokens are consumed when combined into a
** single structure. The TOK_PIPE acts as delimiter
** when consuming tokens.
*/
int	group_tokens(t_shell *shell, t_list **tokens)
{
	t_list	*cmd_node;
	t_list	*node;
	t_token	*token;

	if (init_cmd_nodes(shell, &cmd_node) == SYS_ERROR)
		return (SYS_ERROR);
	node = *tokens;
	while (node)
	{
		token = (t_token *)node->content;
		if (!node->next || token->type == TOK_PIPE)
		{
			if (reset_cmd_node(shell, &cmd_node) == SYS_ERROR)
				return (error_with_cleanup(tokens));
		}
		else
		{
			if (consume_token(cmd_node, token) == SYS_ERROR)
				return (error_with_cleanup(tokens));
		}
		move_and_unlink_token(tokens, &node);
	}
	return (1);
}

/*
** TESTS // TODO resolving quotes:
**	- export TEST="cat -e";ls|"$TEST"
**	- export TEST="cat -e";ls|$TEST
**	- ""<OUT2	-> char **cmd should not be NULL
**	- <OUT2		-> char **cmd should be NULl because empty TOK_WORD
*/

/*
**	2. It adds empty TOK_WORD tokens to REDIR + WORD combinations	// TODO
**	   - i.e. "< IN", which is TOK_LESS->TOK_WORD to TOK_WORD->TOK_LESS->TOK_WORD
**	   This is to help creating the CMD nodes during the grouping phase.
*/

/*
** parse_input_string() checks the incoming string
** from readline() and converts it into a list of
** t_node structures. readline may return NULL if
** it finds an EOF. In this case this function returns
** 0 and breaks the read loop in main.
**
** It does this by:
**
**	1. 		lexing / tokenizing
**	1.1.	scanning
**			- generating simple tokens based on input_string
**				- TOK_WORD is already grouped here
**			- grouping single tokens like > or < into << >>
**			- Checking for syntax errors (*1)
**				- multi-line commands
**				- unknown redir tokens
**				- unknown dollar implementation
**	1.2		evaluating
**			- removing TOK_SPACE tokens
**			- checking for syntax errors (*2)
**				- pipe position check
**	2.		parsing
**  2.1		expansion
**			- expanding environment variables
**	2.2.	normalization
**			- converting aliases to simple form
**	2.2		aggregation
**			- grouping tokens into t_node structures
**	2.3		here-docs
**			- resolving REDIR_DELIM into REDIR_IN nodes
**				- resolving TOK_DOLLAR inside as well
**
** RETURN
**	- 0 if EOF, 1 if successful, -1 if sys_error
*/

int	parse_input_string(char *input_string, t_shell *shell)
{
	t_list	*tokens;

	if (!input_string)					// TODO print exit on EOF ?
		return (0);
	if (!(*input_string))				// if just an ENTER readline() returns "" empty string
		return (1);
	tokens = tokenize(input_string);
	if (!tokens)						// TODO some exit code struct?
		return (0);
	if (!redir_merge(tokens))			// TODO some exit code struct?
		return (0);
	if (!correct_dollar(tokens))		// TODO some exit code struct?
		return (0);
	remove_spaces(&tokens);
	if (!validate_pipes(tokens))		// TODO some exit code struct?
		return (0);
	if (resolve_dollar(shell, &tokens) == SYS_ERROR)
		return (0);						// TODO some exit code struct?
	if (resolve_quotes(&tokens) == SYS_ERROR)
		return (0);
	normalize(&tokens);

	token_display_stdout(tokens);

	// resolve here-doc in parser after initial nodes are made
	// convert the "<< {DELIM}" to a "< {temp.filename}" node.

	return (1);
}
