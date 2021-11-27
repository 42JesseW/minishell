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

/* initialize shell.cmd_nodes by creating a new empty node */
static int	init_cmd_nodes(t_list **cmd_node)
{
	t_list	*cmd;
	t_node	*node;

	node = node_new_def();
	if (!node)
		return (SYS_ERROR);
	cmd = ft_lstnew(node);
	if (!cmd)
	{
		node_del(node);
		return (SYS_ERROR);
	}
	*cmd_node = cmd;
	return (1);
}

static void	move_and_unlink_token(t_list **tokens, t_list **node)
{
	t_list	*unlinked;

	*node = (*node)->next;
	unlinked = ft_lstunlink(tokens, *tokens);
	ft_lstdelone(unlinked, token_del);
}

/*
** is_redir_file() checks if TOK_WORD does not
** belong to redir.file. This would be the case
** if redir.file is still NULL since every valid
** redirect must have a corresponding TOK_WORD.
**
** If above condition is true, *last_redir will
** point to the the last redir structure in the
** list held by {node}.
*/
static bool	is_redir_file(t_node *node, t_redir **last_redir)
{
	t_redir	*redir;
	t_list	*last;

	redir = NULL;
	last = ft_lstlast(node->redir);
	if (last)
	{
		redir = (t_redir *)last->content;
		if (!redir->file)
		{
			*last_redir = redir;
			return (true);
		}
	}
	return (false);
}

static char	**allocate_new_cmd(t_list *token_node)
{
	t_list	*node;
	int		word_amount;
	char	**cmd;

	word_amount = 0;
	node = token_node;
	while (((t_token *)node->content)->type == TOK_WORD)
	{
		node = node->next;
		word_amount++;
	}
	cmd = (char **)malloc(sizeof(char *) * (word_amount + 1));
	if (!cmd)
		return (NULL);
	while (word_amount >= 0)
	{
		cmd[word_amount] = NULL;
		word_amount--;
	}
	return (cmd);
}

static int	consume_word_token(t_node *node, t_list *token_node, int *word_idx)
{
	t_token	*token;
	t_redir	*redir;

	redir = NULL;
	token = (t_token *)token_node->content;
	if (is_redir_file(node, &redir))
	{
		redir->file = ft_strdup(token->token);
		if (!redir->file)
			return (SYS_ERROR);
	}
	else
	{
		if (!node->cmd)
		{
			node->cmd = allocate_new_cmd(token_node);
			if (!node->cmd)
				return (SYS_ERROR);
		}
		node->cmd[(*word_idx)] = ft_strdup(token->token);
		if (!node->cmd[(*word_idx)])
			return (SYS_ERROR);
		(*word_idx)++;
	}
	return (1);
}

/* token.type (t_token_type) maps directly to node.type (t_redir_type). */
static int	consume_redir_token(t_node *node, t_token *token)
{
	t_list	*redir_node;
	t_redir	*redir;

	redir = redir_new_val((t_redir_type)token->type, NULL);
	if (!redir)
		return (SYS_ERROR);
	redir_node = ft_lstnew(redir);
	if (!redir_node)
	{
		redir_del(redir);
		return (SYS_ERROR);
	}
	ft_lstadd_back(&node->redir, redir_node);
	return (1);
}

/*
** consume_token() adds data to the new cmd_node
** based on the token type. TOK_WORD tokens are
** added to node.cmd and redirect tokens are added
** to the list node.redir.
**
** For TOK_WORD nodes the following is checked:
**	1.	Is node.cmd already allocated
**	(false)
**		1. Scan forward and count TOK_WORD.
**		2. Allocate for that many strings
**		3. Copy first TOK_WORD into node.cmd
**		4. Increment word_idx
**	(true)
**		1. Copy TOK_WORD into node.cmd
**		2. Increment word_idx
*/
static int	consume_token(t_list *cmd_node, t_list *token_node)
{
	t_token		*token;
	t_node		*node;
	static int	word_idx = 0;

	node = (t_node *)cmd_node->content;
	if (!node->cmd && !node->redir)
		word_idx = 0;
	token = (t_token *)token_node->content;
	if (is_redir_type(token->type, REDIR_ALL))
	{
		if (consume_redir_token(node, token) == SYS_ERROR)
			return (SYS_ERROR);
	}
	if (token->type == TOK_WORD)
	{
		if (consume_word_token(node, token_node, &word_idx) == SYS_ERROR)
			return (SYS_ERROR);
	}
	return (1);
}

/*
** reset_cmd_node() does the following:
**	1. Check if we're at the last token
**	(true)
**		1. consume the last token so its added to
**		   to the node structure.
**	(false)
**		1. create a new node structure that can be
**		   filled with more tokens.
**	2. In all cases, the *cmd_node is added to the
**	   the list of nodes in shell.cmd_nodes
*/
static int	reset_cmd_node(t_shell *shell, t_list **cmd_node, t_list *token_node)
{
	t_list	*cmd;
	t_node	*node;

	if (!token_node->next)
	{
		if (consume_token((*cmd_node), token_node) == SYS_ERROR)
			return (SYS_ERROR);
	}
	cmd = *cmd_node;
	ft_lstadd_back(&shell->cmd_nodes, cmd);
	*cmd_node = NULL;
	if (!token_node->next)
	{
		node = node_new_def();
		*cmd_node = ft_lstnew(node);
		if (!(*cmd_node) || !(*cmd_node)->content)
		{
			ft_lstdelone((*cmd_node), node_del);
			return (SYS_ERROR);
		}
	}
	return (1);
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
	t_list	*token_node;
	t_token	*token;

	if (init_cmd_nodes(&cmd_node) == SYS_ERROR)
		return (SYS_ERROR);
	token_node = *tokens;
	while (token_node)
	{
		token = (t_token *)token_node->content;
		if (!token_node->next || token->type == TOK_PIPE)
		{
			if (reset_cmd_node(shell, &cmd_node, token_node) == SYS_ERROR)
				return (error_with_cleanup(tokens));
		}
		else
		{
			if (consume_token(cmd_node, token_node) == SYS_ERROR)
				return (error_with_cleanup(tokens));
		}
		move_and_unlink_token(tokens, &token_node);
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
