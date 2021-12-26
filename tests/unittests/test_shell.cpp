#include <catch.hpp>

extern "C" {
	#include <minishell.h>
}

static const char	*envp[] = {
	"__INTELLIJ_COMMAND_HISTFILE__=/Users/jevan-de/Library/Application Support/JetBrains/CLion2020.2/terminal/history/history-25",
	"HOME=/Users/jevan-de",
	"ZDOTDIR=",
	"__CF_USER_TEXT_ENCODING=0x10BA2:0x0:0x0",
	"LOGIN_SHELL=1",
	"PATH=/Users/jevan-de/.brew/bin:/Users/jevan-de/.brew/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki",
	"USER=jevan-de",
	"SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.vIgSz1Rghr/Listeners",
	"TMPDIR=/var/folders/zz/zyxvpxvq6csfxvn_n0008bm80022x2/T/",
	"SHELL=/bin/zsh",
	"LOGNAME=jevan-de",
	"XPC_SERVICE_NAME=0",
	"LC_CTYPE=en_US.UTF-8",
	"Apple_PubSub_Socket_Render=/private/tmp/com.apple.launchd.ziwpMyIfRM/Render",
	"XPC_FLAGS=0x0",
	"TERM=xterm-256color",
	"TERMINAL_EMULATOR=JetBrains-JediTerm",
	"SHLVL=1",
	"ZSH=/Users/jevan-de/.oh-my-zsh",
	"PAGER=less",
	"LESS=-R",
	"LSCOLORS=Gxfxcxdxbxegedabagacad",
	"VARONE=",
	"VARTWO=a=b=c=d",
	"_=/usr/bin/env",
};

SCENARIO("initialize a shell structure from entrypoint") {
	GIVEN("an uninitialised input_line and *envp[]") {
		t_shell	*shell;

		WHEN("creating a new shell structure") {
			shell = shell_init(envp);
			REQUIRE(shell != NULL);
			THEN("the members and input_line should have the appropriate values") {
				t_pair	*pair;
				char	*kv;

				CHECK(shell->cmd_nodes == NULL);
				CHECK(shell->environ != NULL);
				int	idx = 0;
				for (t_list *env = shell->environ; env != NULL; env = env->next, idx++)
				{
					pair = (t_pair *)env->content;
					kv = pair_join(pair);
					if (strcmp(pair->key, "SHLVL") != 0) {
						CHECK(strcmp(envp[idx], kv) == 0);
					}
					free(kv);
				}
			}
			shell_destroy(&shell);
		}
	}
}

SCENARIO("convert an array of strings to a list environment variables") {
	GIVEN("an empty array") {
		const char	**empty_envp = NULL;

		WHEN("creating a list of environment variables") {
			t_list	*environ;

			environ = NULL;
			REQUIRE(environ_from_envp(&environ, empty_envp) == EXIT_SUCCESS);
			THEN("the pointer to the first node should be NULL") {
				REQUIRE(environ == NULL);
			}
		}
	}

	GIVEN("an array with a full list of environment variables") {
		WHEN("creating a list of environment variables") {
			t_list	*environ;

			environ = NULL;
			REQUIRE(environ_from_envp(&environ, envp) == EXIT_SUCCESS);
			THEN("The new list should contain all environment variables") {
				t_pair	*pair;
				char	*kv;
				int		idx;

				idx = 0;
				for (t_list *cur = environ; cur != NULL; cur = cur->next, idx++)
				{
					pair = (t_pair*)cur->content;
					kv = pair_join(pair);
					CHECK(strncmp(envp[idx], kv, strlen(kv)) == 0);
					free(kv);
				}
			}
			ft_lstclear(&environ, pair_del);
		}
	}
}

SCENARIO("convert a list of environment variables back to an array of strings") {
	GIVEN("an empty list of t_pair structures") {
		t_list	*environ;

		environ = NULL;
		WHEN("creating an array of strings") {
			char	**my_envp;
			
			my_envp = environ_to_envp(environ);
			REQUIRE(my_envp != NULL);
			THEN("There should be no elements inside") {
				REQUIRE(*my_envp == NULL);
			}
			ft_strarrfree(&my_envp);
		}
		WHEN("retrieving data from the list") {
			const char	key[] = "SHLVL";
			const char	*value;

			value = environ_get(environ, key);
			THEN("no value should be returned") {
				REQUIRE(value == NULL);
			}
		}
	}

	GIVEN("a full list of t_pair structures") {
		t_list	*environ;
		int		size;

		environ = NULL;
		REQUIRE(environ_from_envp(&environ, envp) == EXIT_SUCCESS);
		size = ft_lstsize(environ);
		WHEN("creating an array of strings") {
			char	**my_envp;

			my_envp = environ_to_envp(environ);
			REQUIRE(my_envp != NULL);
			THEN("The elements inside of the array should match those in the list") {
				t_pair	*pair;
				char	*kv;
				int		idx;

				idx = 0;
				for (t_list *cur = environ; cur != NULL; cur = cur->next, idx++)
				{
					pair = (t_pair*)cur->content;
					kv = pair_join(pair);
					CHECK(strncmp(envp[idx], kv, strlen(kv)) == 0);
					free(kv);
				}
			}
			ft_strarrfree(&my_envp);
		}
		WHEN("retrieving the first item in the list") {
			t_pair		*pair;
			const char	*val;

			pair = pair_new_val(envp[0]);
			val = environ_get(environ, pair->key);
			REQUIRE(val != NULL);
			THEN("this should have the corresponding value") {
				REQUIRE(strncmp(pair->val, val, strlen(val)) == 0);
			}
		}
		WHEN("retrieving the middle item in the list") {
			t_pair		*pair;
			const char	*val;

			pair = pair_new_val(envp[size / 2]);
			val = environ_get(environ, pair->key);
			REQUIRE(val != NULL);
			THEN("this should have the corresponding value") {
				REQUIRE(strncmp(pair->val, val, strlen(val)) == 0);
			}
		}
		WHEN("retrieving the last item in the last") {
			t_pair		*pair;
			const char	*val;

			pair = pair_new_val(envp[size - 1]);
			val = environ_get(environ, pair->key);
			REQUIRE(val != NULL);
			THEN("this should have the corresponding value") {
				REQUIRE(strncmp(pair->val, val, strlen(val)) == 0);
			}
		}
		ft_lstclear(&environ, pair_del);
	}
}
