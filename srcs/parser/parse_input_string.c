/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_input_string.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/14 16:21:32 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/14 16:21:32 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
** write_heredoc() reads from STDIN using readline
** and appends the data to TMP_FILE_NAME using {fd}
** until the line returned by readline matches the
** {delimiter} string.
*/

static int	write_heredoc(char *file_path, char *delimiter)
{
	char	*line;
	ssize_t	ret;
	int		fd;

	fd = open(file_path, O_WRONLY | O_APPEND);
	if (fd == -1)
		return (SYS_ERROR);
	line = readline(HEREDOC_PROMPT);
	while (line)
	{
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ret = write(fd, line, ft_strlen(line));
		free(line);
		if (ret == SYS_ERROR)
			return (SYS_ERROR);
		line = readline(HEREDOC_PROMPT);
	}
	return (1);
}

/*
** convert_heredoc() converts REDIR_DELIM nodes
** to REDIR_IN nodes in the following way:
**	1. remove the TMP_FILE_NAME file if exists
*/

static int	convert_heredoc(t_redir *node)
{
	char	file_path[PATH_MAX];
	char	cwd[PATH_MAX];
	size_t	cwd_len;
	int		fd;

	if (!getcwd(cwd, PATH_MAX))
		return (SYS_ERROR);
	cwd_len = ft_strlen(cwd);
	ft_strcpy(file_path, cwd);
	ft_strlcpy(file_path + cwd_len, TMP_FILE_NAME, ft_strlen(TMP_FILE_NAME));
	fd = open(file_path, O_WRONLY | O_TRUNC | O_CREAT);
	if (fd == -1)
		return (SYS_ERROR);
	close(fd);
	if (write_heredoc(file_path, node->file) == SYS_ERROR)
		return (SYS_ERROR);
	close(fd);
	fd = open(file_path, O_RDONLY);
	if (fd == -1 || unlink(file_path) < 0)
		return (SYS_ERROR);
	node->type = REDIR_IN;
	node->file = ft_strdup(file_path);
	if (!node->file)
		return (SYS_ERROR);
	return (fd);
}

static int	create_file(t_redir *node)
{
	int	mode;
	int	fd;

	mode = 0644;
	if (node->type == REDIR_DELIM)
		fd = convert_heredoc(node);
	else if (node->type == REDIR_IN)
		fd = open(node->file, O_RDONLY);
	else if (node->type == REDIR_OUT)
		fd = open(node->file, O_CREAT | O_TRUNC | O_WRONLY, mode);
	else
		fd = open(node->file, O_CREAT | O_APPEND | O_WRONLY, mode);
	if (fd == -1)
		return (SYS_ERROR);
	return (1);
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

	cmd_node = shell->cmd_nodes;
	while (cmd_node)
	{
		node = (t_node *)cmd_node->content;
		redir_node = node->redir;
		while (redir_node)
		{
			redir = (t_redir *)redir_node->content;
			if (create_file(redir) == SYS_ERROR)
				return (SYS_ERROR);
			redir_node = redir_node->next;
		}
		cmd_node = cmd_node->next;
	}
	return (1);
}

/*
** TESTS // TODO resolving quotes:
**	- export TEST="cat -e";ls|"$TEST"
**	- export TEST="cat -e";ls|$TEST
**	- ""<OUT2	-> char **cmd should not be NULL
**	- <OUT2		-> char **cmd should be NULl because empty TOK_WORD
*/

/*
**	2. It adds empty TOK_WORD tokens to REDIR + WORD combinations	// TODO
**	   - i.e. "< IN", which is TOK_LESS->TOK_WORD to TOK_WORD->TOK_LESS->TOK_WORD
**	   This is to help creating the CMD nodes during the grouping phase.
*/

/*
** parse_input_string() checks the incoming string
** from readline() and converts it into a list of
** t_node structures. readline may return NULL if
** it finds an EOF. In this case this function returns
** 0 and breaks the read loop in main.
**
** It does this by:
**
**	1. 		lexing / tokenizing
**	1.1.	scanning
**			- generating simple tokens based on input_string
**				- TOK_WORD is already grouped here
**			- grouping single tokens like > or < into << >>
**			- Checking for syntax errors (*1)
**				- multi-line commands
**				- unknown redir tokens
**				- unknown dollar implementation
**	1.2		evaluating
**			- removing TOK_SPACE tokens
**			- checking for syntax errors (*2)
**				- pipe position check
**	2.		parsing
**  2.1		expansion
**			- expanding environment variables
**	2.2.	normalization
**			- converting aliases to simple form
**	2.2		aggregation
**			- grouping tokens into t_node structures
**	2.3		here-docs
**			- resolving REDIR_DELIM into REDIR_IN nodes
**				- resolving TOK_DOLLAR inside as well
**
** RETURN
**	- 0 if EOF, 1 if successful, -1 if sys_error
*/

int	parse_input_string(char *input_string, t_shell *shell)
{
	t_list	*tokens;

	if (!input_string)					// TODO print exit on EOF ?
		return (0);
	if (!(*input_string))				// if just an ENTER readline() returns "" empty string
		return (1);
	tokens = tokenize(input_string);
	if (!tokens)						// TODO some exit code struct?
		return (0);
	if (!redir_merge(tokens))			// TODO some exit code struct?
		return (0);
	if (!correct_dollar(tokens))		// TODO some exit code struct?
		return (0);
	remove_spaces(&tokens);
	if (!validate_pipes(tokens))		// TODO some exit code struct?
		return (0);
	if (resolve_dollar(shell, &tokens) == SYS_ERROR)
		return (0);						// TODO some exit code struct?
	if (resolve_quotes(&tokens) == SYS_ERROR)
		return (0);
	normalize(&tokens);
	if (group_tokens(shell, &tokens) == SYS_ERROR)
		return (0);

	nodes_print_stdout(shell->cmd_nodes);

	// resolve here-doc in parser after initial nodes are made
	// convert the "<< {DELIM}" to a "< {temp.filename}" node.

	return (1);
}
