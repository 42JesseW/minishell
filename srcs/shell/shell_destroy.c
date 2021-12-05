/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_destroy.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 11:16:26 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 11:16:26 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	shell_destroy(t_shell **shell)
{
	t_shell	*p;

	if (!shell)
		return ;
	p = *shell;
	if (p)
	{
		ft_lstclear(&p->environ, pair_del);
		free(p);
	}
	*shell = NULL;
}
