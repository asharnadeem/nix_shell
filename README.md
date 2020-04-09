# Basic *Nix Shell

This is a basic implentation of the *Nix Shell, developed for CMSC 421, Principles of Operating Systems. This was my first foray into C language programming. Below are some design decisions made during the development process as specified by the requirements.

I chose to handle the arguments in my main function, determining which mode to run the shell in based off of how many arguments followed the "./simple_shell" statement. 

When 1 argument was ran, I opened a file and went line by line, parsing each line into the appropriate format needed to run the program. I chose to use "strtok" to tokenize the input and achieve this task. As for invalid commands, I used a kill function to let the parent know that it should also terminate.

To deal with dynamic input and not set a cap on how large it could be, I dynamically allocated memory by reallocing the pointer by 1 every time, holding only as much memory as needed for the users input. This is both memory efficient and lets the input be as long as the user wants.

I included a function that validates the commands entered by the user, exiting when it needs to as well as checking for blank input.