/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   dup_redirect.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/05 11:25:38 by aheister      #+#    #+#                 */
/*   Updated: 2021/12/05 11:25:38 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	dup_redirect_write(int fd)
{
	int	fd_dup;

	fd_dup = dup2(fd, STDOUT_FILENO);
	if (fd_dup == -1)
		return (SYS_ERROR);
	return (SUCCESS);
}

int	dup_redirect_read(int fd)
{
	int	fd_dup;

	if (fd == -1)
		return (SUCCESS);
	fd_dup = dup2(fd, STDIN_FILENO);
	if (fd_dup == -1)
		return (SYS_ERROR);
	return (SUCCESS);
}

int	dup_redirect(t_node *cmd_node)
{
	t_list	*lst_redir;
	t_redir	*redir;

	lst_redir = cmd_node->redir;
	while (lst_redir)
	{
		redir = lst_redir->content;
		if (redir->type == REDIR_IN)
		{
			if (dup_redirect_read(redir->fd) == SYS_ERROR)
				return (SYS_ERROR);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APP)
		{
			if (dup_redirect_write(redir->fd) == SYS_ERROR)
				return (SYS_ERROR);
		}
		lst_redir = lst_redir->next;
	}
	return (SUCCESS);
}
