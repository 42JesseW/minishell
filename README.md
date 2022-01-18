# minishell
As beautiful as a shell

Code named "Shelly", this program compiles to a fully functional, bash-like, shell.
For reference, we therefore reference to the [bash manual](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html).

## What does it do

* Shows a prompt when waiting for a new command.
* Has a working History (Up and Down arrows).
* Searches and launches the right executable (based on the PATH variable or by using relative or absolute path)
* Just like bash, it implements builtins:

| Builtin | flags | Synopsis                | Notes                                                                      |
|---------|-------|-------------------------|----------------------------------------------------------------------------|
| echo    | -n    | echo [STRING]...        |                                                                            |
| cd      |       | cd [dir]                | `cd` called without arguments will go to $HOME. `cd -` will go to $OLDPWD. |
| pwd     |       | pwd                     |                                                                            |
| export  |       | export [name[=word]]... | `export [name[+=word]]` can be used to append word to existing word.       |
| unset   |       | unset [name ...]        |                                                                            |
| env     |       | env                     |                                                                            |
| exit    |       | exit [n]                | actually implemented as `exit [n % 256]`                                   |

* ’ inhibits all interpretation of a sequence of characters.
* " inhibits all interpretation of a sequence of characters except for $.

* Redirections:
  * `<` redirects input.
  * `>` redirects output.
  * `<<` reads input from the current source until a line containing only the delimiter is seen.
  * `>>` redirects output with append mode.

* Pipes
  * `|` The output of each command in the pipeline is connected via a pipe to the input of the next command.

* Environment variables ($ followed by characters) expands to their corresponding values.
* `$?` expands to the exit status of the most recently executed foreground pipeline.
* `CTRL-C` `CTRL-D` `CTRL-\` work like in bash.
* When interactive:
   * `CTRL-C` prints a new prompt on a newline.
   * `CTRL-D` exits the shell.
   * `CTRL-\` does nothing.

## What does it not do
* It does not interpret unclosed quotes or unspecified special characters like \ or ;.
* Implement the `$$` environment variable for retrieving the parent pid
* Implement [ANSI-C Quiting](https://www.gnu.org/software/bash/manual/html_node/ANSI_002dC-Quoting.html) so no $"" business.
* Multiline commands. Commands like `echo "hello there` will give a syntax error because the quotes aren't closed.

## Requirements
The program compiles and runs on both Linux and MacOS. It uses the [readline](https://tiswww.case.edu/php/chet/readline/rltop.html)
library for reading user input from STDIN and providing handy features like command line history and other shortcuts like:
* Reverse search (CTRL + R)
* Jumping back to the start of the command line (CTRL + A)
* Jumping to the back of the command line (CTRL + E)

## Installation
If you follow the following steps you're ready to use shelly:
1. Clone the repository.
2. Use [make](https://www.gnu.org/software/make/) to compile the program. Running the `make` command in the cloned directory should do the job.
The makefile will configure and compile `readline`, which is also added, in the local repository's directory. It will then link al needed libaries
and object files into the executable file `minishell`. You can then execute the executable `./minishell`.
3 (Optional) You can make the use of the program easier by adding the install directory to your PATH. If you're in the repository's directory you
can use the following command for this: `export PATH+=":$PWD".

## Known issues / quirks
* `exit 42 42` will give exit code 0 instead of 1
* `exit 5 | exit 45 | exit 3` will do nothing. Exit code will also be 0

