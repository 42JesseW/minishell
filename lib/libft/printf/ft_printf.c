/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/15 15:56:03 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/10/17 15:33:17 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/printf.h"

/*
** Wrapper for real_printf initializing variadic args
**
**  @ARG    {const char*} format
**  @ARG	{...}
**
**	@RETURN	{int} length
*/

int	ft_printf(const char *format, ...)
{
	va_list		args;
	int			len;

	va_start(args, format);
	len = real_printf(STDOUT_FILENO, format, args);
	va_end(args);
	return (len);
}
