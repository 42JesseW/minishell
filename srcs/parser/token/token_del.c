/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_del.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/14 14:41:33 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/14 14:41:33 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

void	token_del(void *token)
{
	t_token	*p;

	p = token;
	if (p)
	{
		free(p->token);
		free(p);
	}
}
