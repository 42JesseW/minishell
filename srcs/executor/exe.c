/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exe.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 10:49:06 by aheister      #+#    #+#                 */
/*   Updated: 2021/11/12 10:49:06 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


void   execute(void)
{
    int		amount_cmds;
    pid_t	pid;

    amount_cmds = ft_lstsize(shell);
	if (amount_cmds > 1)
		pipe((amount_cmds - 1), shell);
	else
	{
		pid = fork(); // execute in a childprocess to avoid crashing of the program
		if (pid < 0)
			printf("Error"); // error handling
		else if (pid == 0)
			execute_cmd(shell);
		//else if (pid > 0)
			//parent process: niet duidelijk of dat hier nodig is
	}
}
