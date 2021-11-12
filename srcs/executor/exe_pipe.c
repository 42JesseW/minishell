/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exe_pipe.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 12:53:05 by aheister      #+#    #+#                 */
/*   Updated: 2021/11/12 12:53:05 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//mallocs an array for the fds

int **malloc_fds(int amount)
{
	int	idx;
	int **fds;

	fds = (int **)malloc(sizeof(int *) * (amount + 1));
	if (!fds)
		printf("Error"); // error handling
	idx = 0;
	while (idx < amount)
	{
		fds[idx] = (int *) malloc(sizeof(int) * 2);
		if (!fds[idx])
			break; // error handling
		idx++;
	}
	fds[amount] = NULL;
	return(fds);
}

void pipe(int amount);
{
	int		idx;
	pid_t	pid;

	idx = 0;
	while (idx < amount)
	{
		if
	}

}