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

#include <parser.h>

t_redir	*redir_new_def(void)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = NULL;
	redir->type = REDIR_IN;
	redir->next = NULL;
	return (redir);
}

t_redir	*redir_new_cpy(t_redir *cpy)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = cpy->type;
	redir->file = NULL;
	if (cpy->file)
		redir->file = ft_strdup(cpy->file);
	if (cpy->file && !redir->file)
	{
		redir_del(&redir);
		return (NULL);
	}
	redir->next = cpy->next;
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
	redir->next = NULL;
	return (redir);
}
