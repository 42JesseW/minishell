/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   child_process.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/27 15:05:27 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/27 15:05:27 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <exe.h>

void	dup_pipe_write(int idx, t_exe *exe)
{
	int fd_out;

	fd_out = dup2(exe->pipe_fds[idx][1], STDOUT_FILENO);
	if (fd_out == -1)
		printf("Error - Duplicating fd failed"); // error handling
}

void	dup_pipe_read(int idx, t_exe *exe)
{
	int fd;

	fd = dup2(exe->pipe_fds[idx][0], STDIN_FILENO);
	if (fd == -1)
		printf("Error - Duplicating fd failed"); // error handling
}

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
