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
	"PWD=/Users/jevan-de/Documents/projects/minishell/shelly/tests",
	"OLDPWD=/Users/jevan-de/Documents/projects/minishell/shelly",
	"ZSH=/Users/jevan-de/.oh-my-zsh",
	"PAGER=less",
	"LESS=-R",
	"LSCOLORS=Gxfxcxdxbxegedabagacad",
	"VARONE=",
	"VARTWO=a=b=c=d",
	"_=/usr/bin/env",
};

SCENARIO("Creating new env structures") {
	GIVEN("a pair") {
		t_pair	*pair;

		pair = pair_new_kv("key", "value");
		REQUIRE(pair != NULL);
		WHEN("constructing a new env structure") {
			t_env	*env;

			env = env_new_val(pair);
			REQUIRE(env != NULL);
			THEN("all members should have the appropriate values") {
				CHECK(env->next == NULL);
				CHECK(strcmp(env->pair->key, pair->key) == 0);
				CHECK(strcmp(env->pair->val, pair->val) == 0);
			}
			env_del(&env);
		}
	}

	GIVEN("another env structure") {
		t_env	*cpy;
		t_pair	*pair;

		pair = pair_new_kv("key", "value");
		REQUIRE(pair != NULL);
		cpy = env_new_val(pair);
		cpy->next = cpy;
		REQUIRE(cpy != NULL);
		WHEN("constructing a new env structure") {
			t_env	*env;

			env = env_new_cpy(cpy);
			REQUIRE(env != NULL);
			THEN("all members should have the appropriate values") {
				CHECK(env->next == cpy);
				CHECK(strcmp(env->pair->key, cpy->pair->key) == 0);
				CHECK(strcmp(env->pair->val, cpy->pair->val) == 0);
			}
			env_del(&env);
		}
		env_del(&cpy);
	}

	GIVEN("no additional information") {
		WHEN("constructing a new env structure") {
			t_env	*env;

			env = env_new_def();
			REQUIRE(env != NULL);
			THEN("all members should have the appropriate values") {
				CHECK(env->next == NULL);
				CHECK(env->pair == NULL);
			}
			env_del(&env);
		}
	}
}

SCENARIO("Using env structures in a list") {
	GIVEN("An empty list of env structures") {
		t_env	*environ;

		environ = NULL;
		THEN("the length of the list should be 0") {
			CHECK(env_lst_len(environ) == 0);
		}
		WHEN("deleting the list") {
			env_lst_del(&environ);
			THEN("the root node should be NULL") {
				CHECK(environ == NULL);
			}
		}
		WHEN("retrieving any env structure") {
			t_env	*env;

			env = env_lst_get(&environ, "key");
			THEN("no node should be retrieved") {
				CHECK(env == NULL);
			}
		}
		WHEN("adding a new env structure") {
			t_env	*env;
			t_env	*env_new;
			t_pair	*pair;

			pair = pair_new_kv("key", "val");
			REQUIRE(pair != NULL);
			env = env_new_val(pair);
			REQUIRE(env != NULL);
			env_new = env_lst_put(&environ, env);
			THEN("the length of the list should be 1") {
				CHECK(env_lst_len(environ) == 1);
			}
			THEN("the root node should be the new node") {
				CHECK(environ == env);
			}
			THEN("the returned node should be the new node") {
				CHECK(env == env_new);
			}
		}
		WHEN("adding multiple new env structures") {
			t_env	*env;
			t_pair	*pair;
			int		idx;

			for (idx = 0; envp[idx] != NULL; idx++)
			{
				pair = pair_new_val(envp[idx]);
				REQUIRE(pair != NULL);
				env = env_new_val(pair);
				REQUIRE(env != NULL);
				env_lst_put(&environ, env);
			}
			THEN("values should correspond with envp[] values") {
				t_env	*env_found;

				for (idx = 0; envp[idx] != NULL; idx++)
				{
					pair = pair_new_val(envp[idx]);
					env_found = env_lst_get(&environ, pair->key);
					REQUIRE(env_found != NULL);
					REQUIRE(strcmp(pair_join(env_found->pair), envp[idx]) == 0);
				}
			}
		}
		env_lst_del(&environ);
	}

	GIVEN("A list with a large number of env structures") {
		t_env	*environ;

		environ = NULL;
		REQUIRE(env_from_envp(&environ, envp) != SYS_ERROR);
		THEN("the length of the list should be the length of the array") {
			CHECK(env_lst_len(environ) == (sizeof(envp) / sizeof(char*)));
		}
		WHEN("deleting the list") {
			env_lst_del(&environ);
			THEN("the list should be empty") {
				CHECK(environ == NULL);
				CHECK(env_lst_len(environ) == 0);
			}
		}
		WHEN("retrieving the first env structure") {
			t_env	*env_first;
			char	*kv;
			char	*key;
			char	*p;

			key = (!(p = strchr((char*)envp[0], '=')))
					? strdup(envp[0])
					: strndup(envp[0], p - envp[0]);
			env_first = env_lst_get(&environ, key);
			REQUIRE(env_first != NULL);
			THEN("env_first should point to root node") {
				CHECK(environ == env_first);
			}
			THEN("the joined pair should match that of the first item in envp") {
				kv = pair_join(env_first->pair);
				REQUIRE(kv != NULL);
				CHECK(strcmp(kv, envp[0]) == 0);
				free(kv);
			}
		}
		WHEN("retrieving the middle env structure") {
			t_env	*env_mid;
			char	*key;
			char	*kv;
			char	*p;
			int		idx;

			idx = (sizeof(envp) / sizeof(char*)) * 0.5;
			key = (!(p = strchr((char*)envp[idx], '=')))
				  ? strdup(envp[idx])
				  : strndup(envp[idx], p - envp[idx]);
			env_mid = env_lst_get(&environ, key);
			REQUIRE(env_mid != NULL);
			THEN("the joined pair should match that of the middle item in envp") {
				kv = pair_join(env_mid->pair);
				REQUIRE(kv != NULL);
				CHECK(strcmp(kv, envp[idx]) == 0);
				free(kv);
			}
		}
		WHEN("retrieving the last env structure") {
			t_env	*env_last;
			char	*key;
			char	*kv;
			char	*p;
			int		idx;

			idx = (sizeof(envp) / sizeof(char*)) - 1;
			key = (!(p = strchr((char*)envp[idx], '=')))
				  ? strdup(envp[idx])
				  : strndup(envp[idx], p - envp[idx]);
			env_last = env_lst_get(&environ, key);
			REQUIRE(env_last != NULL);
			THEN("the joined pair should match that of the last item in envp") {
				kv = pair_join(env_last->pair);
				REQUIRE(kv != NULL);
				CHECK(strcmp(kv, envp[idx]) == 0);
			}
		}
		WHEN("adding a new unique env structure") {
			t_env	*env;
			t_pair	*pair;

			pair = pair_new_kv("IM", "NEW");
			REQUIRE(pair != NULL);
			env = env_new_val(pair);
			REQUIRE(env != NULL);
			env_lst_put(&environ, env);
			pair_del(&pair);
		}
		WHEN("adding a new env structure with duplicate key where both have values") {
			t_env	*env;
			t_pair	*pair;
			t_env	*env_found;

			pair = pair_new_kv("VARTWO", "d=c=b=a");
			REQUIRE(pair != NULL);
			env = env_new_val(pair);
			REQUIRE(env != NULL);
			env_found = env_lst_get(&environ, "VARTWO");
			REQUIRE(env_found != NULL);
			REQUIRE(strcmp(env_found->pair->val, "a=b=c=d") == 0);
			env_lst_put(&environ, env);
			env_found = env_lst_get(&environ, "VARTWO");
			REQUIRE(env_found != NULL);
			REQUIRE(strcmp(env_found->pair->val, "d=c=b=a") == 0);
		}
		WHEN("adding a new env structure with a value where the existing does not have one") {
			t_env	*env;
			t_pair	*pair;
			t_env	*env_found;

			pair = pair_new_kv("VARONE", "VALUE");
			REQUIRE(pair != NULL);
			env = env_new_val(pair);
			REQUIRE(env != NULL);
			env_found = env_lst_get(&environ, "VARONE");
			REQUIRE(env_found != NULL);
			REQUIRE(strlen(env_found->pair->val) == 0);
			env_lst_put(&environ, env);
			env_found = env_lst_get(&environ, "VARONE");
			REQUIRE(env_found != NULL);
			REQUIRE(strcmp(env_found->pair->val, "VALUE") == 0);
			pair_del(&pair);
		}
		env_lst_del(&environ);
	}
}

SCENARIO("environment is initialised from entrypoint main") {
	GIVEN("an incoming array of strings *envp[]") {

		WHEN("env_from_envp() is used to convert the char* to a t_env list") {
			t_env	*environ;
			char	*kv;

			environ = NULL;
			REQUIRE(env_from_envp(&environ, envp) != SYS_ERROR);
			THEN("*environ should contain the converted list") {
				REQUIRE(environ != NULL);
				int	idx = 0;
				for (t_env *env = environ; env != NULL; env = env->next, idx++)
				{
					kv = pair_join(env->pair);
					CHECK(strcmp(envp[idx], kv) == 0);
					free(kv);
				}
			}
			env_lst_del(&environ);
		}
	}
}

SCENARIO("list of env structures is converted back to envp[] list of strings") {

	GIVEN("an empty list of env structures") {
		t_env	*environ;

		environ = NULL;
		WHEN("env_to_envp() is used to convert back to char**") {
			char	**strs;

			strs = env_to_envp(environ);
			THEN("this should return a NULL pointer") {
				CHECK(ft_strarrlen(strs) == 0);
			}
			ft_strarrfree(&strs);
		}
	}

	GIVEN("a full list of env structures") {
		t_env	*environ;

		environ = NULL;
		REQUIRE(env_from_envp(&environ, envp) != SYS_ERROR);
		WHEN("env_to_envp() is used to convert back to char**") {
			char	**arr;

			arr = env_to_envp(environ);
			REQUIRE(arr != NULL);
			THEN("the length of the list should be equal to the length of amount of nodes") {
				char	*kv;
				int		idx;

				idx = 0;
				CHECK(strarrlen(arr) == env_lst_len(environ));
				for (t_env *env = environ; env != NULL; env = env->next, idx++)
				{
					kv = pair_join(env->pair);
					CHECK(strcmp(kv, arr[idx]) == 0);
					free(kv);
				}
			}
		}
		env_lst_del(&environ);
	}
}
