/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_del.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 12:05:23 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 12:05:23 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

void	redir_del(void *redir)
{
	t_redir	*p;

	p = redir;
	if (p)
	{
		free(p->file);
		free(p);
	}
}
