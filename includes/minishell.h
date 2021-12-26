/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/26 11:48:35 by jessevander...#+#    #+#                 */
/*   Updated: 2021/12/26 11:48:35 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# define SYS_ERROR -1
# define NONFATAL 0
# define SUCCESS 1

# define B "\e[0;38;2;218;165;32m"
# define R "\e[m"

# define DEFAULT_PROMPT "shelly3.2$ "
# define HEREDOC_PROMPT "> "
# define SHELL_NAME "shelly"
# ifdef REGRESSION_TEST
#  undef SHELL_NAME
#  define SHELL_NAME "bash"
# endif
# define FMT_ERR ": %s: %s\n"
# define SYNTAX_ERR ": syntax error near unexpected token `%s'\n"
# define INPUTRC_OPTION "set echo-control-characters Off\n"

# include <parser.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <libft/includes/libft.h>
# include <libft/includes/printf.h>
# include <libft/includes/get_next_line.h>

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
	int				fd;
}	t_redir;

/* cmd node for linked list */
typedef struct s_node
{
	char	**cmd;
	t_list	*redir;
}	t_node;

/* key pair representing an environment variable */
typedef struct s_pair
{
	char	*key;
	char	*val;
}	t_pair;

typedef struct s_shell
{
	int		exit_code;
	t_list	*environ;
	t_list	*cmd_nodes;
}	t_shell;

typedef struct s_exe
{
	int		**pipe_fds;
	char	**envp;
	int		amount_cmds;
	t_list	**environ;
	t_list	*pids;
	t_list	*paths;
	t_list	*builtins;
}	t_exe;

typedef struct s_builtin
{
	char		*name;
	int			(*function)(char **, t_exe *);
}	t_builtin;

void		pair_del(void *pair);

char		*pair_join(t_pair *pair);

t_pair		*pair_new_def(void);
void		*pair_new_cpy(void *cpy);
t_pair		*pair_new_kv(const char *key, const char *val);
t_pair		*pair_new_val(const char *kv);

int			environ_from_envp(t_list **root, const char **envp);
char		**environ_to_envp(t_list *root);
const char	*environ_get(t_list *environ, const char *key);
int			environ_update(t_list **environ, char *key, const char *val);
t_list		*environ_new(char *key, const char *val);
void		environ_remove(t_list **environ, char *key);

void		shell_destroy(t_shell **shell);
t_shell		*shell_init(const char *envp[]);
int			init_inputrc(void);
void		set_signals(bool is_parent);

void		redir_del(void *redir);
t_redir		*redir_new_def(void);
void		*redir_new_cpy(void *cpy);
t_redir		*redir_new_val(t_redir_type type, const char *file, int fd);

void		node_del(void *node);
t_node		*node_new_def(void);
void		*node_new_cpy(void *cpy);
t_node		*node_new_val(char **cmd, t_list *redir);

int			parse_input_string(char *input_string, t_shell *shell);
int			consume_token(t_list *cmd_node, t_list *token_node);
int			group_tokens(t_shell *shell, t_list **tokens);
int			resolve_dollar(t_list *environ, t_list **tokens);
char		*resolve_dollar_heredoc(t_list *environ, char *line);
int			create_redir_files(t_shell *shell);

void		nodes_print_stdout(t_list *cmd_nodes);

// BUILTINS
int			builtin_check(int idx, t_node *node, t_exe *exe);
int			builtin_echo(char **cmd, t_exe *exe);
int			builtin_env(char **cmd, t_exe *exe);
int			builtin_pwd(char **cmd, t_exe *exe);
int			builtin_unset(char **cmd, t_exe *exe);
int			builtin_cd(char **cmd, t_exe *exe);
int			init_builtins(t_exe *exe);

// INITIALISATION
int			init_exe(t_shell *shell);
int			init_paths(t_exe *exe, t_shell *shell);
int			store_paths(const char *strpaths, t_exe *exe);
int			prepare_execution(t_exe *exe, t_shell *shell);

// PIPING
int			malloc_fds(t_exe *exe);
void		free_pipe_fds(int **pipe_fds);
int			pipe_loop(t_exe *exe, t_shell *shell);

// DUPPING
int			dup_pipes(int idx, int is_builtin, t_exe *exe);
int			dup_pipe_write(int idx, int is_builtin, t_exe *exe);
int			dup_pipe_read(int idx, int is_builtin, t_exe *exe);
int			dup_redirect(t_node *cmd_node);
int			dup_redirect_read(int fd);
int			dup_redirect_write(int fd);

// FORKING
int			child_process(int idx, t_exe *exe, t_node *cmd_node);
int			fork_process(int idx, t_exe *exe, t_node *cmd_node);
int			close_pipe_ends(int **pipes_fds, int idx);

// EXECUTION
char		*get_full_path(char *cmd, t_exe *exe);
void		execute_cmd(char **cmd, t_exe *exe);
int			execute_single(t_shell *shell, char *argv[]);

#endif
