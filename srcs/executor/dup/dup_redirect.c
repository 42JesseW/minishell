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

void	dup_redirect_write(char *file)
{
	int	fd;
	int	fd_file;

	fd = dup2(fd_file, STDOUT_FILENO);
	if (fd == -1)
		printf("Error - Duplicating fd failed");
//	close(fd_file); // File moet ergens gesloten worden, maar pas als de cmd uitgevoerd is.
}

void	dup_redirect_read(char *file)
{
	int	fd;
	int	fd_file;

	fd = dup2(fd_file, STDIN_FILENO);
	if (fd == -1)
		printf("Error - Duplicating fd failed");
	//close(fd_file); // File moet ergens gesloten worden, maar pas als de cmd uitgevoerd is.
}

int	dup_redirect(t_node *cmd_node)
{

}