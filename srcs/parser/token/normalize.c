/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   normalize.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/25 16:03:39 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/25 16:03:39 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

/* unlink node and insert at the start of part */
void	move_to_front(t_list **tokens, t_list *node, t_bound *bound)
{
	t_list	*unlinked;
	t_list	*word;
	int		idx;

	idx = ft_lstget_idx(*tokens, node);
	unlinked = ft_lstunlink(tokens, node);
	bound->end = ft_lstget_node(*tokens, idx - 1);
	idx = ft_lstget_idx(*tokens, bound->start);
	word = bound->start;
	while (((t_token *)word->content)->type == TOK_WORD)
	{
		word = word->next;
		idx++;
	}
	if (word == bound->start)
		bound->start = unlinked;
	ft_lstinsert(tokens, unlinked, idx);
}

/*
** normalize_part() normalizes a sub commandline:
**	< Makefile cat | > OUT cat > OUT
** |________________________________|
**            normalize()
** |______________| |_______________|
** normalize_part() normalize_part()
**
** The function looks for a pattern:
** [TOK_REDIR] [TOK_WORD] [TOK_WORD]
**
** In the above specified order. When the pattern
** is found, move_to_front() will unlink the TOK_WORD
** node and insert it at the start of the "part".
**
** The insertion must happen after the last TOK_WORD
** starting from the beginning of the part.
*/
void	normalize_part(t_list **tokens, t_bound *bound)
{
	t_token	*token;
	t_list	*node;
	int		count;

	count = 0;
	node = bound->start;
	while (node != bound->next)
	{
		token = (t_token *)node->content;
		if (is_redir_type(token->type, REDIR_ALL) && count == 2)
			count = 1;
		else if ((is_redir_type(token->type, REDIR_ALL) && count == 0)
			 || (token->type == TOK_WORD && count == 1))
			count++;
		else
		{
			if (count == 2)
			{
				move_to_front(tokens, node, bound);
				normalize_part(tokens, bound);
			}
		}
		node = node->next;
	}
}

/*
** normalize() has two main jobs:
**	1. It converts aliased combinations to a
**	   standard configuration. These are some examples:
**	   [<] [FILE] [cat] 						-> [cat] [<] [FILE]
**	   [<] [FILE1] [>] [FILE2] [cat]			-> [cat] [<] [FILE1] [>] [FILE2]
**	   [>] [FILE1] [cat] [<] [FILE2]			-> [cat] [>] [FILE1] [<] [FILE2]
**	   [<] [FILE] [cat] [-e]					-> [cat] [-e] [<] [FILE]
**	   [<] [FILE1] [cat] [-e] [>] [FILE2]		-> [cat] [-e] [<] [FILE1] [>] [FILE2]
**	   [cat] [<] [FILE] [-e]					-> [cat] [-e] [<] [FILE]
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
*/

static void	bound_init(t_bound *bound, t_list *start)
{
	bound->prev = NULL;
	bound->start = start;
	bound->end = NULL;
	bound->next = NULL;
}

void	normalize(t_list **tokens)
{
	t_list	*node;
	t_bound	bound;

	bound_init(&bound, *tokens);
	while (bound.start != NULL)
	{
		node = bound.start;
		if (((t_token *)node->content)->type == TOK_PIPE)
		{
			bound.prev = node;
			bound.start = node->next;
			node = node->next;
		}
		while (node->next && ((t_token *)node->next->content)->type != TOK_PIPE)
			node = node->next;
		bound.end = node;
		bound.next = bound.end->next;
		normalize_part(tokens, &bound);
		bound.start = bound.next;
	}
}
