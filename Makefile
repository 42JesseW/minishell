NAME = minishell

SHELL = /bin/bash

RL_VERSION	= 7.0

LIB_DIR		= lib

LIBFTDIR	= lib/libft
LIBFTLIB	= libft.a
LIB_DIR		+= $(LIBFTDIR)

SOURCE_DIR 	= srcs
OBJECT_DIR 	= obj
INCLUDE_DIR	= includes $(LIBFTDIR)/includes

CMAKE_DIR	= cmake-build

# https://ftp.gnu.org/gnu/readline/readline-7.0.tar.gz
RL_SETUP	= 1
RL_DIR		= $(PWD)/lib/readline-$(RL_VERSION)
ifeq ("$(wildcard $(RL_DIR)/libreadline.a)", "")
	RL_SETUP	= 0
endif

INCLUDE_DIR		+= $(PWD)/lib/readline-$(RL_VERSION)/include
LIB_DIR			+= $(PWD)/lib/readline-$(RL_VERSION)/lib

CLINKS 		= -ltermcap -lreadline -lft
CFLAGS		= -Wall -Wextra -Werror -g -fsanitize=address

ifdef TESTRUN
  CFLAGS	+= -D TESTRUN
endif

SOURCES		= shell/environ/pair/pair_del.c \
			  shell/environ/pair/pair_new.c \
			  shell/environ/pair/pair_join.c \
			  shell/environ/environ_from_envp.c \
			  shell/environ/environ_to_envp.c \
			  shell/environ/environ_get.c \
			  shell/environ/environ_new.c \
			  shell/environ/environ_update.c \
			  shell/environ/environ_remove.c \
			  shell/shell_init.c \
			  shell/shell_exit.c \
			  shell/shell_destroy.c \
			  shell/shell_noninteractive.c \
			  shell/init_inputrc.c \
			  shell/signals.c \
			  parser/node/node_del.c \
			  parser/node/node_new.c \
			  parser/redir/redir_del.c \
			  parser/redir/redir_new.c \
			  parser/redir/redir_merge.c \
			  parser/redir/redir_type.c \
			  parser/token/token_new.c \
			  parser/token/token_del.c \
			  parser/token/token_utils.c \
			  parser/token/tokenize.c \
			  parser/token/correct_dollar.c \
			  parser/token/remove_spaces.c \
			  parser/token/validate_syntax.c \
			  parser/token/resolve_quotes.c \
			  parser/token/normalize.c \
			  parser/token/consume_token.c \
			  parser/token/group_tokens.c \
			  parser/token/insert_merge_token.c \
			  parser/token/is_insert_pos.c \
			  parser/resolve_dollar.c \
			  parser/resolve_dollar_heredoc.c \
			  parser/create_redir_files.c \
			  parser/parse_input_string.c \
			  executor/builtins/bi_check.c \
			  executor/builtins/bi_echo.c \
			  executor/builtins/bi_env.c \
			  executor/builtins/bi_exit.c \
			  executor/builtins/bi_init.c \
			  executor/builtins/bi_pwd.c \
			  executor/builtins/bi_cd.c \
			  executor/builtins/bi_export.c \
			  executor/builtins/bi_export_utils.c \
			  executor/builtins/bi_unset.c \
			  executor/dup/dup_pipes.c \
			  executor/dup/dup_redirect.c \
			  executor/execute/execute.c \
			  executor/fork/fork.c \
			  executor/path/path_of_cmd.c \
			  executor/path/path_init.c \
			  executor/pipe/pipe.c \
			  executor/pipe/pipe_close.c \
			  executor/pipe/pipe_fds_del.c \
			  executor/pipe/pipe_malloc_fds.c \
			  executor/executor.c \
			  main.c

OBJECTS 	= $(addprefix $(OBJECT_DIR)/$(SOURCE_DIR)/, $(SOURCES:.c=.o))
LIBS		= $(addprefix -L, $(LIB_DIR))
INCLUDES	= $(addprefix -I, $(INCLUDE_DIR))

# https://unix.stackexchange.com/questions/269077/tput-setaf-color-table-how-to-determine-color-codes
R=$(shell tput setaf 1)
G=$(shell tput setaf 2)
Y=$(shell tput setaf 3)
P=$(shell tput setaf 5)
B=$(shell tput setaf 4)
W=$(shell tput setaf 7)
BR=$(shell tput setaf 136)

U=$(shell tput smul)
N=$(shell tput sgr0)

define ASCII

    $W▒▒▒▒▒▒▒╗$W▒▒╗  ▒▒╗$W▒▒▒▒▒▒▒╗$W▒▒╗     $W▒▒╗  $W▒▒╗   ▒▒╗
    $W▒▒╔════╝$W▒▒║  ▒▒║$W▒▒╔════╝$W▒▒║     $W▒▒║  $W╚▒▒╗ ▒▒╔╝     ${BR}.════.$W    @   @
    $W▒▒▒▒▒▒▒╗$W▒▒▒▒▒▒▒║$W▒▒▒▒▒╗  $W▒▒║     $W▒▒║   $W╚▒▒▒▒╔╝     ${BR}/ .═"═.`.$W   \\v/
    $W╚════▒▒║$W▒▒╔══▒▒║$W▒▒╔══╝  $W▒▒║     $W▒▒║    $W╚▒▒╔╝      ${BR}║ ║ '\ \ \$W_/ )
    $W▒▒▒▒▒▒▒║$W▒▒║  ▒▒║$W▒▒▒▒▒▒▒╗$W▒▒▒▒▒▒▒╗$W▒▒▒▒▒▒▒╗$W▒▒║     $W,-${BR}\ `═.' /$W.'  /
    $W╚══════╝$W╚═╝  ╚═╝$W╚══════╝$W╚══════╝$W╚══════╝$W╚═╝    $W'---${BR}`════'$W----'
$W               -- created by aheister & jevan-de --

endef
export ASCII

all: ascii $(NAME)

ascii:
	@echo -e "$$ASCII"

$(NAME): $(OBJECTS) $(LIBFTLIB) readline-install
	@$(CC) $(OBJECTS) -o $@ $(LIBS) $(CLINKS) $(INCLUDES) $(CFLAGS)
	@printf "[$(G)INFO$(W)]: Finished building program $(NAME)\n"

$(OBJECT_DIR)/%.o: %.c
	@mkdir -p $(OBJECT_DIR)/$(SOURCE_DIR)/shell/environ/pair
	@mkdir -p $(OBJECT_DIR)/$(SOURCE_DIR)/parser/{node,redir,token}
	@mkdir -p $(OBJECT_DIR)/$(SOURCE_DIR)/executor/{builtins,dup,execute,fork,path,pipe}
	@if $(CC) -c -o $@ $< $(INCLUDES) $(CFLAGS); then \
		printf "[$(G)INFO$(W)]: Successfully created object file %-33.33s\r" $@; \
	else \
	  	printf "\n[$(R)ERROR$(W)]: Failed to create object file %-33.33s\n" $@; \
	  	$(CC) $(INCLUDES) -c $(CFLAGS) -o $@ $<; \
	fi

$(LIBFTLIB):
	@make --directory=$(LIBFTDIR)

readline-install:
	@if [ "$(RL_SETUP)" -eq "0" ]; then \
		cd $(RL_DIR) && ./configure --prefix="$(RL_DIR)" --exec-prefix="$(RL_DIR)"; \
		cd $(RL_DIR) && make install; \
  	fi

readline-uninstall:
	@make --directory=$(RL_DIR) distclean

clean:
	@rm -rf $(OBJECT_DIR) $(CMAKE_DIR)
	@make --directory=$(LIBFTDIR) clean

fclean: clean
	@rm -f $(NAME) $(LIBFTLIB)
	@make --directory=$(LIBFTDIR) fclean

re: fclean all

regression-test:
	@make fclean
	@make TESTRUN=1
	@tests/minishell_tester.sh ./$(NAME)

test:
	@mkdir -p $(CMAKE_DIR)
	@cd $(CMAKE_DIR) && cmake ../ && cmake --build .
	@$(CMAKE_DIR)/tests/tests
