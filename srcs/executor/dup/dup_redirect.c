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

# include <minishell.h>

// deze functie opent de file voor een redirect

void	dup_redirect_write(char *file)
{
	int fd;
	int fd_file;

	fd_file = open(file, O_RDONLY);
	if (fd_file < 0)
		printf("Error - File cannot be opened"); // error handling
	fd = dup2(fd_file, STDOUT_FILENO);
	if (fd == -1)
		printf("Error - Duplicating fd failed"); // error handling
//	close(fd_file); // File moet ergens gesloten worden, maar pas als de cmd uitgevoerd is.
}

void	dup_redirect_read(char *file)
{
	int fd;
	int fd_file;

	fd_file = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_file < 0)
		printf("Error - File cannot be opened"); // error handling
	fd = dup2(fd_file, STDIN_FILENO);
	if (fd == -1)
		printf("Error - Duplicating fd failed"); // error handling
	//close(fd_file); // File moet ergens gesloten worden, maar pas als de cmd uitgevoerd is.
}
