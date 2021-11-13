/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pair_new.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 16:42:14 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 16:42:14 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_pair	*pair_new_def(void)
{
	t_pair	*pair;

	pair = (t_pair *)malloc(sizeof(t_pair));
	if (!pair)
		return (NULL);
	pair->key = NULL;
	pair->val = NULL;
	return (pair);
}

void	*pair_new_cpy(void *cpy)
{
	t_pair	*pair;
	t_pair  *copy;

	pair = (t_pair *)malloc(sizeof(t_pair));
	if (!pair)
		return (NULL);
	copy = cpy;
	pair->key = NULL;
	if (copy->key)
		pair->key = ft_strdup(copy->key);
	pair->val = NULL;
	if (copy->val)
		pair->val = ft_strdup(copy->val);
	if ((copy->key && !pair->key) || (copy->val && !pair->val))
	{
		free(pair->key);
		free(pair->val);
		return (NULL);
	}
	return (pair);
}

/*
** wrapper for pair_new_kv() that splits
** a complete key value string into its
** pair.key and pair.val components.
*/

t_pair	*pair_new_val(const char *kv)
{
	char	*key;
	char	*val;
	char	*p;

	val = NULL;
	p = ft_strchr(kv, '=');
	if (!p)
		key = ft_strdup(kv);
	else
	{
		key = ft_strndup(kv, (p - kv));
		val = ft_strdup(kv + (p - kv) + 1);
	}
	if ((!p && !key) || (p && (!key || !val)))
	{
		free(key);
		free(val);
		return (NULL);
	}
	return (pair_new_kv(key, val));
}

t_pair	*pair_new_kv(const char *key, const char *val)
{
	t_pair	*pair;

	pair = (t_pair *)malloc(sizeof(t_pair));
	if (!pair)
		return (NULL);
	pair->key = NULL;
	if (key)
		pair->key = ft_strdup(key);
	pair->val = NULL;
	if (val)
		pair->val = ft_strdup(val);
	if ((key && !pair->key) || (val && !pair->val))
	{
		free(pair->key);
		free(pair->val);
		return (NULL);
	}
	return (pair);
}
