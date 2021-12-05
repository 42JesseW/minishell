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

TEST_CASE("testing init_exe") {
	t_shell	*shell;

	shell = shell_init(envp);
	REQUIRE(shell != NULL);
}
