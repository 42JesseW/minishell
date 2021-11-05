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

t_pair	*pair_new_cpy(t_pair *cpy)
{
	t_pair	*pair;

	pair = (t_pair *)malloc(sizeof(t_pair));
	if (!pair)
		return (NULL);
	pair->key = NULL;
	if (cpy->key)
		pair->key = strdup(cpy->key);		// TODO use ft_strdup
	pair->val = NULL;
	if (cpy->val)
		pair->val = strdup(cpy->val);		// TODO use ft_strdup
	if ((cpy->key && !pair->key) || (cpy->val && !pair->val))
	{
		free(pair->key);
		free(pair->key);
		return (NULL);
	}
	return (pair);
}

t_pair	*pair_new_val(const char *key, const char *val)
{
	t_pair	*pair;

	pair = (t_pair *)malloc(sizeof(t_pair));
	if (!pair)
		return (NULL);
	pair->key = NULL;
	if (key)
		pair->key = strdup(key);			// TODO use ft_strdup
	pair->val = NULL;
	if (val)
		pair->val = strdup(val);			// TODO use ft_strdup
	if ((key && !pair->key) || (val && !pair->val))
	{
		free(pair->key);
		free(pair->key);
		return (NULL);
	}
	return (pair);
}
