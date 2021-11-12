/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_lst_add.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 12:52:27 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 12:52:27 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

/* lst_add_back for redir structure */
void	redir_lst_add(t_redir **root, t_redir *redir)
{
	t_redir	*traverse_redir;

	if (!root)
		return ;
	if (!(*root))
		*root = redir;
	else
	{
		traverse_redir = *root;
		while (traverse_redir)
		{
			if (!traverse_redir->next)
			{
				traverse_redir->next = redir;
				break ;
			}
			traverse_redir = traverse_redir->next;
		}
	}
}
