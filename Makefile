NAME = minishell

SHELL = /bin/bash

LIBFTDIR	= libft
LIBFTLIB	= libft.a

SOURCE_DIR 	= srcs
OBJECT_DIR 	= obj
INCLUDE_DIR	= includes $(LIBFTDIR)/includes
CMAKE_DIR	= cmake-build

CLINKS 		= -ltermcap -lreadline
CFLAGS		= -Wall -Wextra -Werror

SOURCES		= env/env_init.c \
			  env/env_del.c \
			  env/env_new.c \
			  env/env_lst_del.c \
			  env/env_lst_get.c \
			  env/env_lst_put.c \
			  pair/pair_del.c \
			  pair/pair_new.c \
			  pair/pair_join.c \
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

        ______       ____         ${BR}.════.$W    @   @
       / __/ /  ___ / / /_ __    ${BR}/ .═"═.`.$W   \\v/
      _\ \/ _ \/ -_) / / // /    ${BR}║ ║ '\ \ \$W_/ )
     /___/_//_/\__/_/_/\_, /  $W,-${BR}\ `═.' /$W.'  /
                      /___/   $W'---${BR}`════'$W----'

$W        -- created by aheister & jevan-de --

endef
export ASCII

all: ascii $(NAME)

setup:
	@mkdir -p $(OBJECT_DIR)/$(SOURCE_DIR)/env
	@mkdir -p $(OBJECT_DIR)/$(SOURCE_DIR)/pair

ascii:
	@echo -e "$$ASCII"

$(NAME): setup $(OBJECTS) $(LIBFTLIB)
	@$(CC) $(INCLUDES) $(CFLAGS) $(OBJECTS) -o $@ -L. $(LIBS) $(CLINKS)
	@printf "[$(G)INFO$(W)]: Finished building program $(NAME)\n"

$(OBJECT_DIR)/%.o: %.c
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
