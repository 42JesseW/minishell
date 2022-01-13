#!/bin/bash

TIMEOUT=2
SIGSEG=139
SIGTIME=124
EXITCODE=0

R=$(tput setaf 1)
G=$(tput setaf 2)
Y=$(tput setaf 3)

U=$(tput smul)
N=$(tput sgr0)

BASH_SHELL=/bin/bash
YOUR_SHELL=$(ps -o comm= $PPID)
SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

# tests check for the following
# 1. exit_code
# 1.1 $SIGSEG   	-> minishell program segfaults
# 1.2 $SIGTIME  	-> minishell program times out when running command
# 1.3 $?        	-> minishell exit_code
# 2. output written to STDOUT
# 3. norminette		-> install using pip

if [[ -n $1 ]]; then
  YOUR_SHELL=$1
  if [[ ! -f "$(dirname "$0")/../Makefile" ]]; then
		printf "[${R}ERROR${N}] Failed to find project makefile"
		exit 1
	fi
	make -C "$SCRIPT_DIR/.." minishell
fi

if [[ ! -f "$(dirname "$0")/tests.txt" ]]; then
  printf "[${R}ERROR${N}] Failed to find tests.txt"
  exit 1;
fi

# Make sure all programs to run the script are installed and available
required_programs=(
  "norminette"
  "timeout"
)
for prog in "${required_programs[@]}"
do
  if ! "$prog" --version &> /dev/null; then
    printf "%-20s%s\n" "${R}Error${N}" "Script needs $prog to function properly"
    exit 1
  fi
done

# Make sure norminette passes
norm_paths=(
	"$SCRIPT_DIR/../libft/includes"
	"$SCRIPT_DIR/../libft/srcs"
	"$SCRIPT_DIR/../srcs"
)
norminette "${norm_paths[@]}" | grep --invert-match "OK" # TODO norminette output parsing
if [[ "$?" -eq 1 ]]; then
	EXITCODE=1
fi

command_lines=()
while IFS= read -r cmd; do command_lines+=("$cmd"); done < "$(dirname "$0")/tests.txt"

# NOTES
# =~ is a regex operator that matches a pattern
# ${var:x:y} is getting substring of $var starting at index x and ending at index y
# ${var#\#} removes all # at the start of the string $var
# <<- is insertion operator but with optional tabs for indentation
# IFS='$c' read -r -a $array_name <<< "$var" creates a new based on splitting string $var using delimiter $c
# "${#arr[@]}" returns the number of elements in array arr
# "${arr[@]:x:y}" gets all elements from array arr between index x and index y
for cmd in "${command_lines[@]}"
do
	# print the Title for a group of tests
	if [[ "${cmd:0:1}" == "#" ]]; then
		printf "%-40s\n" "${Y}${U}${cmd#\#}${N}"
		continue
	# comments
	elif [[ "${cmd:0:2}" == "//" ]]; then
		continue
	# empty lines
  elif [[ -z "$cmd" ]]; then
  	echo
    continue
  fi
  # if the $cmd variable contains a ';' character, multiple commands
  # are specified.
  if [[ "$cmd" =~ .*";".* ]]; then
    IFS=';' read -r -a multi_cmd <<< "$cmd"
    cmd="$(cat <<- EOF
			$(for sub in "${multi_cmd[@]}"; do echo "$sub"; done)
			EOF
			)"
  fi
  # if the $cmd variable contains a ':' character, a HEREDOC is
  # specified.
#  if [[ "$cmd" =~ .*":".* ]]; then
#  	IFS=':' read -r -a heredoc_cmd <<< "$cmd"
#		cmd="$(printf "%s%s\n%s\n%s\n" \
#			"cat << " \
#			"${heredoc_cmd[0]}" \
#			"$(for sub in "${heredoc_cmd[@]:1:${#heredoc_cmd[@]}-2}"; do echo "$sub"; done)" \
#			"${heredoc_cmd[${#heredoc_cmd[@]}-1]}" \
#		)"
#  fi
  bash_output=$(timeout $TIMEOUT echo "$cmd" | $BASH_SHELL 2>&1)
  bash_exit_code=$?
  your_output=$(timeout $TIMEOUT echo "$cmd" | $YOUR_SHELL 2>&1)
  your_exit_code=$?

	# replace '\n' characters with " ; " in $cmd
  printf "%-110s" "${U}[> ${cmd//$'\n'/ ; }]${N}"
  if [[ your_exit_code -eq $SIGSEG ]]; then
    printf "%40s" "${R}SEGFAULT${N}";
    EXITCODE=1
  elif [[ your_exit_code -eq $SIGTIME ]]; then
    printf "%40s" "${R}TIMEOUT${N}";
    EXITCODE=1
  elif ! diff <(echo "$bash_output") <(echo "$your_output"); then
  	printf "%40s" "${R}OUTPUT${N}"
  	EXITCODE=1
  else
    printf "%40s" "${G}SUCCESS${N}"
  fi
  echo
done

exit $EXITCODE

# SPECIAL ENVIRONMENT VARIABLES
# 1. SHLVL      -> Incremented by 1 each time you open the program. Taken from {extern char **environ or char *envp[]}
# 2. PWD        -> Current working directory																	{USE getenv() if unset}
# 3. OLDPWD     -> Previous working directory. Empty when the program starts. {USE getenv() if unset}
# 4. PATH       -> Should be used to find binaries from left to right
#               -> when unset, program can not be found anymore

# EDGE CASES
# only spaces in commandline
# mkdir NOT_EXISTING_DIR ; cd NOT_EXISTING_DIR ; rm -r ../NOT_EXISTING_DIR ; pwd

# EXIT CODES IN BASH ($?)
# CTRL + C          -> 1
# CTRL + \          -> 131
## > cat -
## CTRL + \
## > echo $?
# syntax errors     -> 258
## > ;
## > echo $?
# Command not found -> 127
## > -ls
## echo $?

# BUILTINS
# 1. echo
## echo
## echo -n
## echo -nnnnn
## echo -n -n -n -n
## echo -n -nf
## echo -nf -n

# 2. cd
## cd
### Should go to $HOME. If not set, print "bash: cd: HOME not set" to STDERR_FILENO
### Should check if $PWD is set. If not set, set it to new directory passed to cd
### Should check if $OLDPWD is set. If not, set it to current directory before chdir(new path)
## cd NOT_EXISTING_FILE
## cd EXISTING_DIR
## cd EXISTING_FILE
## cd EXISTING_DIR EXISTING_DIR
## cd NOT_EXISTING_DIR EXISTING_DIR

# 3. export
## export
### Should print the environment variables lexicographically sorted with 'declare -x' in front.
## export TEST
## export TEST=
## export TEST=a
## export TEST=a=b=c=d=e=f=g=h=i=j
### first character of the key must comply with {isalpha(c) || c == '_'}
### the other characters in the key must comply with {isalnum(c)}
## export TEST+=
## export TEST+=a
## export TEST+=b

# 4. env
## env
### Only print environment variables with both a key and a value

# 5. exit
## exit
### exit code 0
## exit 42
### exit code 42
## exit 42 42
### error "too many arguments".
## exit exit exit
### error numeric argument required
### exit code 255
## exit exit 42
### error numeric argument required
### exit code 255
## exit 42 exit
### error "too many arguments"

# 6. unset
## unset
## unset NOT_EXISTING_KEY
## unset EXISTING_KEY
## unset _1
## unset 1
### first character of the key must comply with {isalpha(c) || c == '_'}
### the other characters in the key must comply with {isalnum(c)}

# 7. pwd
## pwd
### Use getcwd() to retrieve the current working directory


# REDIRECTIONS ( duplicate > for >> )

# Aliases
#-------------------
## < EXISTING_FILE cat                                -> cat < EXISTING_FILE
## > NOT_EXISTING_FILE cat -e < EXISTING_FILE         -> cat -e < EXISTING_FILE > NOT_EXISTING_FILE
# > FILE_ONE echo "TEXT" > FILE_TWO > FILE_THREE			-> echo "TEXT" > FILE_ONE > FILE_TWO > FILE_THREE

# Multiple redirects
#-------------------
## echo "TEXT" > FILE_ONE > FILE_TWO > FILE_THREE
## > FILE_ONE echo "TEXT" > FILE_TWO > FILE_THREE
## > FILE_ONE > FILE_TWO > FILE_THREE

## < EXISTING_FILE < EXISTING_FILE < EXISTING_FILE
## < NOT_EXISTING_FILE < EXISTING_FILE < EXISTING_FILE
## < EXISTING_FILE < NOT_EXISTING_FILE < EXISTING_FILE
## < EXISTING_FILE < EXISTING_FILE < NOT_EXISTING_FILE

## < EXISTING_FILE > NOT_EXISTING_FILE
## <> EXISTING_FILE grep "SOMETHING_IN_EXISTING_FILE" > OUTFILE

# Single and only
#-------------------
## < EXISTING_FILE
## < NOT_EXISTING_FILE
## > EXISTING_FILE
## > NOT_EXISTING_FILE


# PIPES

# Weird endings
#-------------------
## |
## echo test |
## | echo test

# One pipe
#-------------------
## echo test | grep test
## cat EXISTING_FILE | wc -l

# Multiple pipes
#-------------------
## echo one | cat -e | cat -e | grep one
## cat EXISTING_FILE | grep SOMETHING_IN_EXISTING_FILE | wc -l
## echo test | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e

# With redirections
#-------------------
## cat < Makefile | cat -e > OUT

# With redirections and aliases
#-------------------
## < Makefile cat | cat -e > OUT
## cat < Makefile | > OUT cat -e
## < Makefile cat | > OUT cat -e
