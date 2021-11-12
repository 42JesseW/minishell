/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_lst_del.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 13:06:35 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 13:06:35 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

void	redir_lst_del(t_redir **root)
{
	t_redir	*redir;
	t_redir	*temp;

	if (!root)
		return ;
	redir = *root;
	while (redir)
	{
		temp = redir;
		redir = redir->next;
		redir_del(&temp);
	}
	*root = NULL;
}
