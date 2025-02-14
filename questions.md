1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice for reading user input in our shell because it safely reads an entire line, including spaces, 
    and ensures we do not exceed buffer limits by specifying a maximum length. Additionally, fgets() allows for proper EOF detection, 
    making it useful for handling interactive and non-interactive shell usage.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Using malloc() to allocate memory for cmd_buff instead of a fixed-size array allows for greater flexibility, especially if we want to handle
     variable-length input efficiently. A fixed-size array might waste memory if the input is small or be too small for larger inputs. Dynamic allocation ensures 
     we allocate just enough memory for user input while still allowing for reallocation if needed

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  If we don’t remove extra spaces, our shell might think spaces are part of the command, causing problems. If there’s a space before the command, our shell might not recognize it properly. Trimming spaces makes sure commands 
    are clean and ready to run without weird issues.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Redirection means changing where a command sends its output or gets its input. Normally, a command prints to the screen, but with redirection, we can save it to a file or send errors somewhere else.
    Instead of showing the file list on screen, it saves it to files.txt.
    Challenge: We need to tell our shell to open the file and write to it instead of the screen.
    Both normal output and errors go into output.txt.
    Challenge: Our shell must process the redirections in the right order so that both outputs end up in the same file.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**: Redirection (> and <) sends output to a file or reads input from a file.
                  Piping (|) connects the output of one command to another command. Piping is used to chain commands together instead of saving output to a file.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Imagine you run a command, and it prints both results and errors together. That would be messy!
       If errors and normal output are mixed, it’s harder to process results correctly, especially for scripts. Keeping errors separate lets us handle them properly.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  If a command fails, our shell should show a clear error message and return an error code.
                   If a user wants to combine normal output and errors, we should allow them to merge (2>&1).
                   Normal output goes to output.txt. Errors go to error.txt.
        This way, users have full control over what happens to their command results.

