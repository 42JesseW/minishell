/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_type.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/25 16:06:02 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/25 16:06:02 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

bool	is_redir_type(t_token_type type, bool check_single)
{
	bool	single;
	bool	both;

	single = (type == TOK_LESS || type == TOK_GREAT);
	both = (single || type == TOK_DLESS || type == TOK_DGREAT);
	return ((check_single && single) || (!check_single && both));
}
