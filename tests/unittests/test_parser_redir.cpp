#include <catch.hpp>
#include <unittest.hpp>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

extern "C" {
	#include <minishell.h>
}

class CreateRedirFilesFixture : public BaseFixture
{
protected:
	t_shell	*shell;

public:
	CreateRedirFilesFixture()
	{
		shell = shell_init(envp);
		REQUIRE(shell != NULL);
	}

	void	init_tokens(const char *input_string) override {
		tokens = tokenize(input_string);
		REQUIRE(tokens != NULL);
		REQUIRE(redir_merge(tokens));
		REQUIRE(correct_dollar(tokens));
		remove_spaces(&tokens);
		REQUIRE(validate_pipes(tokens));
		REQUIRE(resolve_dollar(shell, &tokens) != SYS_ERROR);
		REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
		normalize(&tokens);
		REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	}
};

TEST_CASE_METHOD(CreateRedirFilesFixture, "REDIR_OUT") {
	std::string	file_name	= "OUT";
	std::string	cmd			= "echo test > ";
	struct stat	buffer;

	init_tokens((cmd + file_name).c_str());
	REQUIRE(create_redir_files(shell) != SYS_ERROR);
	REQUIRE(stat(file_name.c_str(), &buffer) == 0);
	auto	*cmd_node	= (t_node *)shell->cmd_nodes->content;
	auto	*redir_node	= (t_redir *)cmd_node->redir->content;
	REQUIRE(redir_node->fd > 2);
	REQUIRE(close(redir_node->fd) == EXIT_SUCCESS);
	remove(file_name.c_str());
}

TEST_CASE_METHOD(CreateRedirFilesFixture, "REDIR_OUT (multiple)") {
	std::string	file_name		= "OUT";
	std::string	file_data		= "testdata";
	std::string	cmd				= "echo " + file_data;
	int			file_amount		= 4;
	std::vector<std::string>	files(file_amount);
	std::ifstream				out;
	std::string					test_data;
	struct stat					buffer;

	for (int i = 0; i < file_amount; i++) {
		files[i] = (file_name + std::to_string(i));
		cmd += (" > " + files[i]);
	}
	init_tokens(cmd.c_str());
	REQUIRE(create_redir_files(shell) != SYS_ERROR);
	for (auto &file : files) {
		REQUIRE(stat(file.c_str(), &buffer) == 0);
		/* check if files are empty */
		out.open(file);
		REQUIRE(out.peek() == std::ifstream::traits_type::eof());
		out.close();
		remove(file.c_str());
	}
}

TEST_CASE_METHOD(CreateRedirFilesFixture, "REDIR_IN") {
	std::string	file_name	= "IN";
	std::string	cmd			= "cat -e < ";
	std::string	file_data	= "testdata";
	struct stat	buffer;
	int			fd;

	fd = open(file_name.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
	REQUIRE(fd != -1);
	REQUIRE(write(fd, file_data.c_str(), file_data.length()));
	close(fd);
	init_tokens((cmd + file_name).c_str());
	REQUIRE(create_redir_files(shell) != SYS_ERROR);
	REQUIRE(stat(file_name.c_str(), &buffer) == 0);
	auto	*cmd_node	= (t_node *)shell->cmd_nodes->content;
	auto	*redir_node	= (t_redir *)cmd_node->redir->content;
	REQUIRE(redir_node->fd > 2);
	REQUIRE(close(redir_node->fd) == EXIT_SUCCESS);
	remove(file_name.c_str());
}

/* opening this file should not truncate it (O_TRUNC) */
TEST_CASE_METHOD(CreateRedirFilesFixture, "REDIR_APP") {
	std::string	file_name	= "OUT";
	std::string	cmd			= "echo test >> ";
	std::string	file_data	= "testdata";
	struct stat	buffer;
	char		read_buff[file_data.length() + 1];
	int			fd;

	/* write testdata to {file_name} */
	fd = open(file_name.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
	REQUIRE(fd != -1);
	REQUIRE(write(fd, file_data.c_str(), file_data.length()));
	close(fd);
	init_tokens((cmd + file_name).c_str());
	REQUIRE(create_redir_files(shell) != SYS_ERROR);
	REQUIRE(stat(file_name.c_str(), &buffer) == 0);
	auto	*cmd_node	= (t_node *)shell->cmd_nodes->content;
	auto	*redir_node	= (t_redir *)cmd_node->redir->content;
	REQUIRE(redir_node->fd > 2);
	/* read testdata into read_buffer to compare */
	fd = open(file_name.c_str(), O_RDONLY);
	REQUIRE(fd != -1);
	REQUIRE(read(fd, read_buff, file_data.length()) == file_data.length());
	read_buff[file_data.length()] = 0;
	REQUIRE(strcmp(read_buff, file_data.c_str()) == 0);
	close(fd);
	REQUIRE(close(redir_node->fd) == EXIT_SUCCESS);
	remove(file_name.c_str());
}

TEST_CASE_METHOD(CreateRedirFilesFixture, "REDIR_DELIM") {
	std::string	delim		= "EOF";
	std::string	cmd			= "cat << " + delim;
	std::string	file_name	= "IN";
	std::string	file_data	= "testdata";
	ssize_t		file_size	= file_data.length() + delim.length();
	char		read_buff[file_data.length() + 1];
	int			fd;
	int			old_fd;

	/* write testdata to file */
	fd = open(file_name.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
	REQUIRE(fd != -1);
	REQUIRE(write(fd, (file_data + "\n" + delim).c_str(), file_size + 1) != -1);
	close(fd);
	/* make sure readline reads from file instead of STDIN */
	fd = open("IN", O_RDONLY);
	old_fd = dup(STDIN_FILENO);
	REQUIRE(dup2(fd, STDIN_FILENO) == 0);
	init_tokens(cmd.c_str());
	REQUIRE(create_redir_files(shell) != SYS_ERROR);
	auto	*cmd_node	= (t_node *)shell->cmd_nodes->content;
	auto	*redir_node	= (t_redir *)cmd_node->redir->content;
	REQUIRE(redir_node->fd > 2);
	REQUIRE(redir_node->type == REDIR_IN);
	errno = 0;
	REQUIRE((fcntl(redir_node->fd, F_GETFD) != -1 && errno != EBADF));
	/* read testdata into read_buffer to compare */
	REQUIRE(read(redir_node->fd, read_buff, file_data.length()) == file_data.length());
	read_buff[file_data.length()] = 0;
	REQUIRE(strcmp(read_buff, file_data.c_str()) == 0);
	close(fd);
	REQUIRE(close(redir_node->fd) == EXIT_SUCCESS);
	/* reset some stuff */
	REQUIRE(dup2(old_fd, STDIN_FILENO) == 0);
	remove(file_name.c_str());
}