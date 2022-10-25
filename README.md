# Minishell
⭐️ The objective of Minishell is to code a simple shell. Show a prompt when waiting for a new command, search and launch the right executable

## Features ⤵️
- builtins : echo with option -n, cd, pwd, export, unset, env, exit
- environment variables ($followed by characters) expands to their values
- ctrl-C, ctrl-D and ctrl -\
- ' inhibit all interpretation of a sequence of characters
- " inhibit all interpretation of a sequence of characters except for $
- redirections: < redirects input > redirects output >> redirects output with append mode
- pipes | the output of each command in the pipeline is connected via a pipe to the input of the next command
- $? expands to the exit status of the most recently executed foreground pipeline

## 💡 Usage
Run ``Make && ./minishell``
