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

SCENARIO("initialize a shell structure from entrypoint") {
	GIVEN("an uninitialised input_line and *envp[]") {
		t_shell	*shell;
		char	*input_line;

		WHEN("creating a new shell structure") {
			shell = shell_init(envp, &input_line);
			REQUIRE(shell != NULL);
			REQUIRE(input_line != NULL);
			THEN("the members and input_line should have the appropriate values") {
				char	*kv;

				CHECK(shell->cmd_nodes == NULL);
				CHECK(strcmp(input_line, "") == 0);
				CHECK(shell->environ != NULL);
				int	idx = 0;
				for (t_list *env = shell->environ; env != NULL; env = env->next, idx++)
				{
					kv = pair_join((t_pair *)env->content);
					CHECK(strcmp(envp[idx], kv) == 0);
					free(kv);
				}
			}
			shell_destroy(&shell);
		}
	}
}
