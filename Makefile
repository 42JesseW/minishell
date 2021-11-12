NAME = minishell

SHELL = /bin/bash

LIBFTDIR	= libft
LIBFTLIB	= libft.a

SOURCE_DIR 	= srcs
OBJECT_DIR 	= obj
INCLUDE_DIR	= includes $(LIBFTDIR)/includes
CMAKE_DIR	= cmake-build

CLINKS 		= -ltermcap -lreadline
CFLAGS		= -Wall -Wextra -Werror -g -fsanitize=address

SOURCES		= shell/env/pair/pair_del.c \
			  shell/env/pair/pair_new.c \
			  shell/env/pair/pair_join.c \
			  shell/env/env_from_envp.c \
			  shell/env/env_to_envp.c \
			  shell/env/env_del.c \
			  shell/env/env_new.c \
			  shell/env/env_lst_len.c \
			  shell/env/env_lst_del.c \
			  shell/env/env_lst_get.c \
			  shell/env/env_lst_put.c \
			  shell/shell_init.c \
			  shell/shell_destroy.c \
			  main.c

OBJECTS 	= $(addprefix $(OBJECT_DIR)/$(SOURCE_DIR)/, $(SOURCES:.c=.o))
LIBS		= $(addprefix -l, $(subst lib,,$(subst .a,,$(LIBFTLIB))))
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

$(NAME): $(OBJECTS) $(LIBFTLIB)
	@$(CC) $(INCLUDES) $(CFLAGS) $(OBJECTS) -o $@ -L. $(LIBS) $(CLINKS)
	@printf "[$(G)INFO$(W)]: Finished building program $(NAME)\n"

$(OBJECT_DIR)/%.o: %.c
	@mkdir -p $(OBJECT_DIR)/$(SOURCE_DIR)/shell/env/pair
	@if $(CC) $(INCLUDES) -c $(CFLAGS) -o $@ $<; then \
		printf "[$(G)INFO$(W)]: Successfully created object file %-33.33s\r" $@; \
	else \
	  	printf "\n[$(R)ERROR$(W)]: Failed to create object file %-33.33s\n" $@; \
	  	$(CC) $(INCLUDES) -c $(CFLAGS) -o $@ $<; \
	fi

$(LIBFTLIB):
	@make --directory=$(LIBFTDIR)
	@cp $(LIBFTDIR)/$(LIBFTLIB) .

clean:
	@rm -rf $(OBJECT_DIR) $(CMAKE_DIR)
	@make --directory=$(LIBFTDIR) clean

fclean: clean
	@rm -f $(NAME) $(LIBFTLIB)
	@make --directory=$(LIBFTDIR) fclean

re: fclean all

test:
	@mkdir -p $(CMAKE_DIR)
	@cd $(CMAKE_DIR) && cmake ../ && cmake --build .
	@$(CMAKE_DIR)/tests/tests
