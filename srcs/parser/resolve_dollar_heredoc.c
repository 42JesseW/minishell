/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   resolve_dollar_heredoc.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/23 14:13:03 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/23 14:13:03 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*join_tokens(t_list *tokens)
{
	t_list	*token_node;
	t_token	*token;
	char	*resolved_line;
	char	*joined;

	resolved_line = ft_strdup("");
	if (!resolved_line)
		return (NULL);
	token_node = tokens;
	while (token_node)
	{
		token = (t_token *)token_node->content;
		joined = ft_strjoin(resolved_line, token->token);
		free(resolved_line);
		if (!joined)
			return (NULL);
		resolved_line = joined;
		token_node = token_node->next;
	}
	return (resolved_line);
}

char	*resolve_dollar_heredoc(t_shell *shell, char *line)
{
	t_list	*tokens;
	char	*resolved_line;

	tokens = tokenize(line);
	if (!tokens || !correct_dollar_heredoc(tokens))
		return (NULL);
	if (resolve_dollar(shell, &tokens, true) == SYS_ERROR)
	{
		ft_lstclear(&tokens, token_del);
		return (NULL);
	}
	resolved_line = join_tokens(tokens);
	if (!resolved_line)
	{
		ft_lstclear(&tokens, token_del);
		return (NULL);
	}
	ft_lstclear(&tokens, token_del);
	return (resolved_line);
}
