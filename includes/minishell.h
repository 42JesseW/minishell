/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/05 16:38:36 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/05 16:38:36 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H
# define SYS_ERROR -1

# define B "\e[0;38;2;218;165;32m"
# define R "\e[m"

# include <libft.h>
# include <parser.h>
# include <stdio.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>

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

void		pair_del(void *pair);

char		*pair_join(t_pair *pair);

t_pair		*pair_new_def(void);
void		*pair_new_cpy(void *cpy);
t_pair		*pair_new_kv(const char *key, const char *val);
t_pair		*pair_new_val(const char *kv);

int			environ_from_envp(t_list **root, const char **envp);	// TODO testcase
char		**environ_to_envp(t_list *root);						// TODO testcase
const char	*environ_get(t_list *environ, const char *key);

void		shell_destroy(t_shell **shell);
t_shell		*shell_init(const char *envp[], char **input_string);

void		redir_del(void *redir);
t_redir		*redir_new_def(void);
void		*redir_new_cpy(void *cpy);
t_redir		*redir_new_val(t_redir_type type, const char *file);

void		node_del(void *node);
t_node		*node_new_def(void);
void		*node_new_cpy(void *cpy);
t_node		*node_new_val(char **cmd, t_list *redir);

int			parse_input_string(char *input_string, t_shell *shell);	// TODO testcase
int			resolve_dollar(t_shell *shell, t_list **tokens);

#endif
