1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_answer here_1. 
My shell calls `waitpid()` on each child process to wait until they finish. Without `waitpid()`, zombie processes accumulate, and the shell may accept new input too early.  

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_answer here_Closing unused pipe ends frees resources and prevents processes from hanging. Open pipes can cause memory leaks or block processes waiting for input.  

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_answer here_`cd` changes the parent shell’s directory, which can’t be done by an external process. If implemented externally, it would only change the child process’s directory, not the shell’s.  

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_answer here_Use dynamic arrays (e.g., `realloc()`) to store pipes and PIDs. The trade-offs include increased complexity and managing memory carefully to avoid leaks.
