/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_lst_len.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/08 07:12:47 by null          #+#    #+#                 */
/*   Updated: 2021/11/08 07:12:47 by null          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int		env_lst_len(t_env *root)
{
	t_env	*traverse_env;
	int		len;

	len = 0;
	traverse_env = root;
	while (traverse_env)
	{
		traverse_env = traverse_env->next;
		len++;
	}
	return (len);
}
