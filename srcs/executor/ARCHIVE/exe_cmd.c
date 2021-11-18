/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exe_cmd.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 12:55:54 by aheister      #+#    #+#                 */
/*   Updated: 2021/11/12 12:55:54 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void execute_cmd(t_shell **shell)
{
    char	*path;

    while (shell->list_paths)
    {
        path = ft_strjoin(input->list_paths->content, cmd[0]);
        check_malloc(path);
        if (access(path, (F_OK & X_OK)) != -1)
        {
            if (execve(path, cmd, input->envp) == -1)
                error_handling(STDERR_FILENO, "Execution failed.", cmd[0], 2);
        }
        free(path);
        input->list_paths = input->list_paths->next;
    }
    error_handling(STDERR_FILENO, "Command not found.", cmd[0], 2);
}