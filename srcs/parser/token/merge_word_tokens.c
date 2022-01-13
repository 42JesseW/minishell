/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   merge_word_tokens.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/13 09:39:09 by jessevander...#+#    #+#                 */
/*   Updated: 2022/01/13 09:39:09 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/* merge TOK_WORD tokens and unlink right TOK_WORD and TOK_MERGE nodes */
static int	merge_word(t_list **tokens, t_list **node, t_list **prev)
{
	t_token	*left_word;
	t_token	*right_word;
	t_list	*unlinked;
	char	*new_token;

	left_word = (*prev)->content;
	right_word = (*node)->next->content;
	new_token = ft_strjoin(left_word->token, right_word->token);
	if (!new_token)
		return (SYS_ERROR);
	free(left_word->token);
	left_word->token = new_token;
	unlinked = ft_lstunlink(tokens, (*node)->next);
	ft_lstdelone(unlinked, token_del);
	unlinked = ft_lstunlink(tokens, (*node));
	ft_lstdelone(unlinked, token_del);
	*node = (*prev)->next;
	return (1);
}

/*
** Look for TOK_MERGE tokens and merge the two
** TOK_WORD tokens on both sides together to
** create a new TOK_WORD:
**	1. Look for TOK_MERGE token
**	2. Update .prev TOK_WORD using .next
**	3. Unlink .next TOK_WORD node
**	4. unlink the TOK_MERGE node
** tokens from the token list.
*/
int	merge_word_tokens(t_list **tokens)
{
	t_list	*node;
	t_list	*prev;
	t_token	*token;

	prev = NULL;
	node = *tokens;
	while (node)
	{
		token = (t_token *)node->content;
		if (token->type == TOK_MERGE)
		{
			if (merge_word(tokens, &node, &prev) == SYS_ERROR)
				return (SYS_ERROR);
		}
		else
		{
			prev = node;
			node = node->next;
		}
	}
	return (1);
}
