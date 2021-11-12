#ifndef PARSER_H

# define PARSER_H

# include <libft.h>

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
	struct s_redir	*next;
}	t_redir;

/* cmd node for linked list */
typedef struct s_node
{
	char			**cmd;
	t_redir			*redir;
	struct s_node	*next;
}	t_node;

void	redir_del(t_redir **redir);	// TODO testcase

t_redir	*redir_new_def(void);	// TODO testcase
t_redir	*redir_new_cpy(t_redir *cpy);	// TODO testcase
t_redir	*redir_new_val(t_redir_type type, const char *file);	// TODO testcase

void	redir_lst_del(t_redir **root);	// TODO testcase
t_redir	*redir_lst_dup(t_redir *root);	// TODO testcase
void	redir_lst_add(t_redir **root, t_redir *redir); // TODO testcase

void	node_del(t_node	**node);	// TODO testcase
t_node	*node_new_cpy(t_node *cpy);	// TODO testcase
t_node	*node_new_val(char **cmd, t_redir *redir);	// TODO testcase
t_node	*node_new_def(void);	// TODO testcase

#endif
