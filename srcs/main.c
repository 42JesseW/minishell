/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 16:37:36 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 16:37:36 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

#include <readline/readline.h>

static const char	g_prompt[] = "shelly3.2$ ";

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APP,
	REDIR_DELIM
}	t_redir_type;

/* redir node for linked list */
typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redi	*next;
}	t_redir;

/* cmd node for linked list */
typedef struct s_node
{
	char	**args;
	t_redir	*redir;
}	t_node;

/*
** main() has 4 main jobs:
**	1. checking arguments passed to the program
**	2. initialising the t_shell struct
**	3. passing output from the initial readline() call to the lexer
**	4. passing output from the parser to the executor
*/

int	main(int argc, char *argv[], const char *envp[])
{
	t_shell	*shell;
	char	*input_line;

	if (argc > 1 || argv[1])
		return (EXIT_SUCCESS);
	shell = shell_init(envp, &input_line);
	if (!shell)
		return (EXIT_FAILURE);
	while (input_line)
	{
		free(input_line);
		input_line = readline(g_prompt);
		printf("%s\n", input_line);
	}
	return (EXIT_SUCCESS);
}
