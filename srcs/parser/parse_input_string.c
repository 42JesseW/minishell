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

/*
** normalize() has two main jobs:
**	1. It converts aliased combinations to a
**	   standard configuration. These are some examples:
**	   [<] [FILE] [cat] 					-> [cat] [<] [FILE]
**	   [<] [FILE1] [>] [FILE2] [cat]		-> [cat] [<] [FILE1] [>] [FILE2]
**	   [>] [FILE1] [cat] [<] [FILE2]		-> [cat] [>] [FILE1] [<] [FILE2]
**	   [<] [FILE] [cat] [-e]				-> [cat] [-e] [<] [FILE]
**	   [<] [FILE1] [cat] [-e] [>] [FILE2]	-> [cat] [-e] [<] [FILE1] [>] [FILE2]
**	   [cat] [-e] [cat] [<] [FILE1] [>] [FILE2] [>] [FILE3] [cat]	-> [cat -e cat < FILE1 > FILE2 > FILE3]
**	2. It adds empty TOK_WORD tokens to REDIR + WORD combinations
**	   - i.e. "< IN", which is TOK_LESS->TOK_WORD to TOK_WORD->TOK_LESS->TOK_WORD
**	   This is to help creating the CMD nodes during the grouping phase.
**
** in all cases the actual program(s) (with flags) is (are)
** moved to the front of the total command line. This is to
** keep the order correct.
**
** since a command line can be broken up into sub
** commands using pipes, and normalization only applies
** to redir nodes, normalize() uses normalize_part() to
** normalize each sub command line.
**	// TODO each sub command can be normalized recursively
**	// TODO must be done after resolving environment variables
*/

void	normalize(t_list **tokens)
{
	(void)tokens;
}

/*
** environ_get() returns a pointer to the string
** held by the t_pair structure if key matches
** pair.key of an existing environment variable.
*/
const char	*environ_get(t_list *environ, const char *key)
{
	t_list	*traverse;
	t_pair	*pair;

	if (!environ || !key)
		return (NULL);
	traverse = environ;
	while (traverse)
	{
		pair = (t_pair *)traverse->content;
		if (ft_strncmp(pair->key, key, ft_strlen(key)) == 0)
			return (pair->val);
		traverse = traverse->next;
	}
	return (NULL);
}

static void	remove_token(t_list **tokens, t_list *rm)
{
	t_list	*traverse;

	traverse = *tokens;
	if (!traverse)
		return ;
	if (traverse == *tokens)
	{
		*tokens = traverse->next;
		ft_lstdelone(traverse, token_del);
	}
	else
	{
		while (traverse->next != rm)
			traverse = traverse->next;
		traverse->next = rm->next;
		ft_lstdelone(rm, token_del);
	}
}

static int	append_to_token(t_token *new_token, t_list *node)
{
	t_token	*token;
	char	*new_token_str;

	token = (t_token *)node->content;
	new_token_str = ft_strjoin(new_token->token, token->token);
	if (new_token_str)
		return (SYS_ERROR);
	free(new_token->token);
	new_token->token = new_token_str;
	return (1);
}

/*
** resolve() should do the following:
**	1. create a new TOK_WORD node where .token is
**	   created by joining all .token components from
**	   all token nodes in between the matching quote.
**	2. remove all intermediary token nodes.
**	3. return the new token to the caller
*/

static t_token	*resolve(t_list **tokens, t_list *start, t_token_type type)
{
	t_list	*node;
	t_list	*temp;
	t_token	*token;

	token = token_new_val(TOK_WORD, "");
	if (!token)
		return (NULL);
	node = start->next;
	remove_token(tokens, start);
	while (((t_token *)node->content)->type != type)
	{
		if (append_to_token(token, node) == SYS_ERROR)
		{
			token_del(token);
			return (NULL);
		}
		temp = node;
		node = node->next;
		remove_token(tokens, temp);
	}
	remove_token(tokens, node);
	return (token);
}

static int	insert_word(t_list **tokens, t_list *prev, t_token *word)
{
	t_list	*node;

	node = ft_lstnew(word);
	if (!node)
		return (SYS_ERROR);
	if (!prev)
	{
		node->next = (*tokens)->next;
		*tokens = node;
	}
	else
	{
		node->next = prev->next;
		prev->next = node;
	}
	return (1);
}

/*
** resolve_quotes() creates one TOK_WORD out of
** TOK_QUOTE / TOK_DQUOTE and everything inside.
**
** assumes the following:
**	1. If a TOK_DOLLAR is found, this can safely
**	   be converted to a string (only in TOK_QUOTE).
**	2. Every QUOTE token has a matching QUOTE token
**	   of the same type (has_paired_quotes).
*/

int	resolve_quotes(t_list **tokens)
{
	t_list	*node;
	t_list	*prev;
	t_token	*token;
	t_token	*word;

	prev = NULL;
	node = *tokens;
	while (node)
	{
		token = (t_token *)node->content;
		if (token->type == TOK_QUOTE || token->type == TOK_DQUOTE)
		{
			word = resolve(tokens, node, token->type);
			if (!word || !insert_word(tokens, prev, word))
				return (SYS_ERROR);
		}
		prev = node;
		node = node->next;
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

int	parse_input_string(const char *input_string, t_shell *shell)
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
	// TODO resolve quotes
	// TODO normalize (resolve aliases) ?

	token_display_stdout(tokens);

	// resolve here-doc in parser after initial nodes are made
	// convert the "<< {DELIM}" to a "< {temp.filename}" node.

	return (1);
}
