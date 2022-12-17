# ssh(SidSHell) - Linux C shell

## Running and exiting the shell
In the directory where the source file resides,
1. Run the `make` command.
2. Run `./ssh` to start the shell.
3. Enter `exit` to exit the shell.

## Assumptions/Limitations

+ Maximum possible length of command input is `1024`.

+ Maximum number of background processes that can be run across one session = `1024`.

+ The print statements informing the user about the status of background processes may overlap with input prompt or user input. This does not affect the functionality.

+ When background process is called its pid is printed after one prompt

+ Assuming hidden files are not expected as an output of `discover`

+ If `cd -` is run before any change of directory, that is, there is no previous directory, we change the directory to `~`

+ Assuming that no file named `history.txt` in the home folder is not in use by the user or any other process. The contents of this file will be modified.

+  Number of commands history outputs is 20.

