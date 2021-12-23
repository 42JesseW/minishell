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

//void echo_check(char *cmd[], int type)
//{
//	int 	idx;
//	int 	fd;
//	int 	fd_stdout;
//	char 	*system_cmd;
//	char 	*new_system_cmd;
//
//	if (type == 1)
//		fd = open("test1.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
//	else
//		fd = open("test2.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
//	REQUIRE(fd != -1);
//	fd_stdout = dup(STDOUT_FILENO);
//	dup2(fd, STDOUT_FILENO);
//	close(fd);
//	idx = 0;
//	if (type == 1)
//		builtin_echo(cmd);
//	else
//	{
//		system("bash -c 'echo -nnnnnb hallohallohallo hallohallohallo'");
//	}
//	fflush(stdout);
//	dup2(fd_stdout, STDOUT_FILENO);
//	close(fd_stdout);
//}
//
//TEST_CASE("testing builtin echo") {
//	char	*cmd[] = {
//			"echo", "-nnnnnb", "hallohallohallo", "hallohallohallo", NULL
//	};
//	int 	fd;
//	char 	buff1[100];
//	char 	buff2[100];
//	ssize_t	ret;
//
//	echo_check(cmd, 1);
//	echo_check(cmd, 2);
//
//	fd = open("test1.txt", O_RDONLY);
//	ret = read(fd, buff1, 100);
//	buff1[ret] = 0;
//	fd = open("test2.txt", O_RDONLY);
//	ret = read(fd, buff2, 100);
//	buff2[ret] = 0;
//
//	CHECK(strcmp(buff1, buff2) == 0);
//
//	remove("test1.txt");
//	remove("test2.txt");
//}