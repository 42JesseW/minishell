#ifndef PARSER_H

# define PARSER_H

# include <minishell.h>

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
