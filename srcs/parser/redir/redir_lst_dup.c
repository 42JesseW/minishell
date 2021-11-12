/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_lst_dup.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 13:25:42 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 13:25:42 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_redir	*redir_lst_dup(t_redir *root)
{
	t_redir	*dup_root;
	t_redir	*dup;
	t_redir	*redir;

	if (!root)
		return (NULL);
	redir = root;
	while (redir)
	{
		dup = redir_new_cpy(redir);
		if (!dup)
		{
			redir_lst_del(&dup_root);
			return (NULL);
		}
		redir_lst_add(&dup_root, dup);
		redir = redir->next;
	}
	return (dup_root);
}
