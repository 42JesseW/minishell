#include <catch.hpp>

extern "C" {
	#include <minishell.h>
}

SCENARIO("Creating new env structures") {
	GIVEN("a pair") {
		t_pair	*pair;

		pair = pair_new_val("key", "value");
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

		pair = pair_new_val("key", "value");
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

SCENARIO("environment is initialised from entrypoint main") {
	GIVEN("an incoming array of strings *envp[]") {
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
			"PWD=/Users/jevan-de/Documents/projects/minishell/shelldon/tests",
			"OLDPWD=/Users/jevan-de/Documents/projects/minishell/shelldon",
			"ZSH=/Users/jevan-de/.oh-my-zsh",
			"PAGER=less",
			"LESS=-R",
			"LSCOLORS=Gxfxcxdxbxegedabagacad",
			"VARONE=''",
			"VARTWO=a=b=c=d",
			"_=/usr/bin/env",
		};

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
