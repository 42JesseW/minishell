/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 16:38:36 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 16:38:36 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H
# define SYS_ERROR -1

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <libft.h>

typedef struct s_pair
{
	char	*key;
	char	*val;
}	t_pair;

typedef struct s_env
{
	struct s_env	*next;
	t_pair			*pair;
}	t_env;

void	pair_del(t_pair **pair);

char	*pair_join(t_pair *pair);

t_pair	*pair_new_def(void);
t_pair	*pair_new_cpy(t_pair *cpy);
t_pair	*pair_new_val(const char *key, const char *val);

int		env_init(t_env **root, const char *envp[]);

void	env_del(t_env **env);

t_env	*env_new_def(void);
t_env	*env_new_cpy(t_env *cpy);
t_env	*env_new_val(t_pair *pair);

void	env_lst_del(t_env **root);
t_env	*env_lst_put(t_env **root, t_env *env);

#endif
