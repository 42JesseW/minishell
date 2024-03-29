/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   create_redir_files.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/08 13:53:04 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/12/08 13:53:04 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** convert_heredoc() converts REDIR_DELIM nodes
** to REDIR_IN nodes in the following way:
**	1. remove the HEREDOC_FILE file if exists
*/

static int	convert_heredoc(t_shell *shell, t_redir *node, int mode)
{
	char	file_path[PATH_MAX];
	char	cwd[PATH_MAX];
	size_t	cwd_len;
	int		fd;

	if (!getcwd(cwd, PATH_MAX))
		return (SYS_ERROR);
	cwd_len = ft_strlen(cwd);
	ft_strcpy(file_path, cwd);
	ft_strlcpy(file_path + cwd_len, HEREDOC_FILE, ft_strlen(HEREDOC_FILE) + 1);
	fd = open(file_path, O_CREAT | O_TRUNC | O_WRONLY, mode);
	if (fd == -1)
		return (SYS_ERROR);
	close(fd);
	if (write_heredoc(shell, file_path, node->file) == SYS_ERROR)
		return (SYS_ERROR);
	fd = open(file_path, O_RDONLY);
	if (fd == -1 || unlink(file_path) < 0)
		return (SYS_ERROR);
	node->type = REDIR_IN;
	free(node->file);
	node->file = ft_strdup(file_path);
	if (!node->file)
		return (SYS_ERROR);
	return (fd);
}

static int	get_redir_fd(t_shell *shell, t_redir *node)
{
	int	mode;
	int	fd;

	mode = DF_OPEN_PERM;
	if (node->type == REDIR_DELIM)
		fd = convert_heredoc(shell, node, mode);
	else if (node->type == REDIR_IN)
		fd = open(node->file, O_RDONLY);
	else if (node->type == REDIR_OUT)
		fd = open(node->file, O_CREAT | O_TRUNC | O_WRONLY, mode);
	else
		fd = open(node->file, O_CREAT | O_APPEND | O_WRONLY, mode);
	if (fd == -1)
	{
		if (!g_exit_code_sig)
			ft_dprintf(STDERR_FILENO, SHELL_NAME FMT_ERR, node->file,
				strerror(errno));
		else
			ft_lstclear(&shell->cmd_nodes, node_del);
		errno = 0;
		return (NONFATAL);
	}
	return (fd);
}

/*
** create_redir_files() loops all redir nodes for each cmd_node
** in shell.cmd_nodes and does the following:
**	1. REDIR_DELIM is converted to REDIR_IN (see convert_heredoc)
**	2. Get a file descriptor
**		2.1 mode based on the t_redir.type :
**			- REDIR_IN	-> (O_RDONLY)
**			- REDIR_OUT	-> (O_CREAT | O_TRUNC | O_WRONLY)
**			- REDIR_APP	-> (O_CREAT | O_APPEND | O_WRONLY)
**		2.2 file permissions -> 0644
**		2.3 If REDIR_DELIM, unlink the created file descriptor
**
**	3. set t_redir.fd
*/

int	create_redir_files(t_shell *shell)
{
	t_list	*cmd_node;
	t_node	*node;
	t_list	*redir_node;
	t_redir	*redir;
	int		fd;

	cmd_node = shell->cmd_nodes;
	while (cmd_node)
	{
		node = (t_node *)cmd_node->content;
		redir_node = node->redir;
		while (redir_node)
		{
			redir = (t_redir *)redir_node->content;
			fd = get_redir_fd(shell, redir);
			if (fd <= NONFATAL)
				return (fd);
			redir->fd = fd;
			redir_node = redir_node->next;
		}
		cmd_node = cmd_node->next;
	}
	return (SUCCESS);
}
