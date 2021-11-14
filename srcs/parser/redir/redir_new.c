/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_new.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/12 12:13:13 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/12 12:13:13 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_redir	*redir_new_def(void)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = NULL;
	redir->type = REDIR_IN;
	return (redir);
}

void	*redir_new_cpy(void *cpy)
{
	t_redir	*redir;
	t_redir	*copy;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	copy = cpy;
	redir->type = copy->type;
	redir->file = NULL;
	if (copy->file)
		redir->file = ft_strdup(copy->file);
	if (copy->file && !redir->file)
	{
		redir_del(&redir);
		return (NULL);
	}
	return (redir);
}

t_redir	*redir_new_val(t_redir_type type, const char *file)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = NULL;
	if (file)
		redir->file = ft_strdup(file);
	if (file && !redir->file)
	{
		redir_del(&redir);
		return (NULL);
	}
	return (redir);
}
