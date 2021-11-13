#ifndef PARSER_H

# define PARSER_H

# include <minishell.h>

void	redir_del(void *redir);	// TODO testcase
t_redir	*redir_new_def(void);	// TODO testcase
void	*redir_new_cpy(void *cpy);	// TODO testcase
t_redir	*redir_new_val(t_redir_type type, const char *file);	// TODO testcase

void	node_del(void *node);	// TODO testcase
t_node	*node_new_def(void);	// TODO testcase
void	*node_new_cpy(void *cpy);	// TODO testcase
t_node	*node_new_val(char **cmd, t_list *redir);	// TODO testcase

#endif
