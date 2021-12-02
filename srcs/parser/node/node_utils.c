/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/02 14:30:26 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/12/02 14:30:26 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*redir_map[] = {"<", ">", ">>", "<<"};

void	nodes_print_stdout(t_list *cmd_nodes)	// TODO not norm
{
	t_node	*node;
	t_redir	*redir;

	for (t_list *cur = cmd_nodes; cur != NULL; cur = cur->next)
	{
		node = (t_node *)cur->content;
		printf("--------------------\n");
		printf("|%9s %-8s|\n", "cmd", " ");
		if (!node->cmd)
			printf("|%11s %-6s|\n", "(null)", " ");
		else
		{
			for (int idx = 0; node->cmd[idx]; idx++)
				printf("|%6d | %-9s|\n", idx, node->cmd[idx]);
		}
		printf("--------------------\n");
		printf("|[%5s | %8s]|\n", "type", "filename");
		for (t_list *r = node->redir; r != NULL; r = r->next)
		{
			redir = (t_redir *)r->content;
			printf("||%5s | %-8s||\n", redir_map[redir->type], redir->file);
		}
		printf("--------------------\n");
		printf("%9s\n%11s\n", "|", "ᐯ");
	}
}
