#include <catch.hpp>

extern "C" {
	#include <minishell.h>
}

SCENARIO("creating new node structures") {
    GIVEN("no input information") {
        WHEN("creating a new node") {
            t_node  *node;

            node = node_new_def();
            THEN("all members should have the appropriate values") {
                CHECK(node->cmd == NULL);
                CHECK(node->redir == NULL);
            }
        }
    }

    GIVEN("input information for the node") {
        char            *cmd[] = {"/bin/cat", "-e", "filename", NULL};
        char			file[] = "filename";
        t_list 			*root;
        t_list			*lst;
        t_redir			*redir;
        t_redir_type    types[] = {
            REDIR_IN,
            REDIR_APP,
            REDIR_OUT,
            REDIR_DELIM
        };

        root = NULL;
        for (int idx = 0; types[idx] != REDIR_DELIM; idx++)
		{
        	redir = redir_new_val(types[idx], file);
        	REQUIRE(redir != NULL);
        	lst = ft_lstnew(redir);
        	REQUIRE(lst != NULL);
        	ft_lstadd_back(&root, lst);
		}
        REQUIRE(ft_lstsize(root) == 3);
        WHEN("creating a new node") {
            t_node  *node;

            node = node_new_val(cmd, root);
            REQUIRE(node != NULL);
            THEN("all members should have the appropriate values") {
            	t_redir	*r;

            	for (int idx = 0; node->cmd[idx]; idx++)
            		CHECK(strcmp(cmd[idx], node->cmd[idx]) == 0);
            	int idx = 0;
            	for (t_list *n = node->redir; n != NULL; n = n->next, idx++)
				{
            		r = (t_redir*)n->content;
            		CHECK(r->type == types[idx]);
            		CHECK(strcmp(r->file, file) == 0);
				}
            }
            node_del(node);
        }
    }

    GIVEN("a copy of a node") {
		char            *cmd[] = {"/bin/cat", "-e", "filename", NULL};
		char			file[] = "filename";
		t_node  		*cpy;
		t_list 			*root;
		t_list			*lst;
		t_redir			*redir;
		t_redir_type    types[] = {
				REDIR_IN,
				REDIR_APP,
				REDIR_OUT,
				REDIR_DELIM
		};

		root = NULL;
		for (int idx = 0; types[idx] != REDIR_DELIM; idx++)
		{
			redir = redir_new_val(types[idx], file);
			REQUIRE(redir != NULL);
			lst = ft_lstnew(redir);
			REQUIRE(lst != NULL);
			ft_lstadd_back(&root, lst);
		}
		REQUIRE(ft_lstsize(root) == 3);
		cpy = node_new_val(cmd, root);
		REQUIRE(cpy != NULL);
		WHEN("creating a new node") {
			t_node	*node;

			node = (t_node*)node_new_cpy((void*)cpy);
			REQUIRE(node != NULL);
			THEN("all members should have the appropriate values") {
				t_list	*n1;
				t_list	*n2;
				t_redir	*r1;
				t_redir *r2;

				for (int idx = 0; cpy->cmd[idx]; idx++)
					CHECK(strcmp(node->cmd[idx], cpy->cmd[idx]) == 0);
				n1 = node->redir;
				n2 = cpy->redir;
				for (; n1 != NULL && n2 != NULL; n1 = n1->next, n2 = n2->next)
				{
					r1 = (t_redir*)n1->content;
					r2 = (t_redir*)n2->content;
					CHECK(r1->type == r2->type);
					CHECK(strcmp(r1->file, r2->file) == 0);
				}
			}
			node_del(node);
		}
		node_del(cpy);
    }
}
