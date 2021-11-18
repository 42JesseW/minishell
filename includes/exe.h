/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exe.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/18 07:57:08 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/18 07:57:08 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_H
# define EXE_H

# include <minishell.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct s_exe
{
	t_list	*paths;
}	t_exe;

int		init_paths(t_exe *exe, t_shell *shell);
void	store_paths(const char *strpaths, t_exe *exe, t_shell *shell);

#endif
