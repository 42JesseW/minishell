/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: annaheister <annaheister@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/18 08:38:10 by annaheister   #+#    #+#                 */
/*   Updated: 2021/11/18 08:38:10 by annaheister   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// Hier functie uittrekken die execution start: main is tijdelijk

#include <exe.h>

void init_exe(t_shell *shell)
{
	t_exe		*exe;

	exe = (t_exe *)malloc(sizeof(t_exe));
	if (!exe)
		printf("Error - Malloc failed\n");
	if (init_paths(exe, shell) == SYS_ERROR)
		printf("Error - Initialization of paths failed\n");
	while (exe->paths)
	{
		printf("path = %s\n", exe->paths->content);
		exe->paths = exe->paths->next;
	}
}

// gcc -Iincludes srcs/executor/exe.h srcs/executor/exe_paths.c srcs/executor/executor.c srcs/shell/environ/pair/pair_del.c srcs/shell/environ/pair/pair_join.c srcs/shell/environ/pair/pair_new.c srcs/shell/environ/environ_from_envp.c includes/minishell.h libft/includes/libft.h libft/srcs/ft_bzero.c libft/srcs/ft_calloc.c libft/srcs/lst/ft_lstadd_back.c libft/srcs/lst/ft_lstclear.c libft/srcs/lst/ft_lstdelone.c libft/srcs/lst/ft_lstnew.c libft/srcs/ft_split.c libft/srcs/str/ft_strarrfree.c libft/srcs/str/ft_strchr.c libft/srcs/str/ft_strcpy.c libft/srcs/str/ft_strdup.c libft/srcs/str/ft_strjoin.c libft/srcs/str/ft_strlcat.c libft/srcs/str/ft_strlcpy.c libft/srcs/str/ft_strncmp.c libft/srcs/str/ft_strlen.c libft/srcs/str/ft_strndup.c libft/srcs/ft_substr.c srcs/shell/shell_init.c srcs/shell/shell_destroy.c