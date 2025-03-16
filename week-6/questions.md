1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_answer here_  The remote client can determine the full output by:
Using a delimiter (like newline \n) to mark the end of the message.
Sending the length of the message before the data.
Reading in chunks and ensure the complete message is received.
To handle partial reads, use non-blocking I/O or select() and accumulate data until the full message is received.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_answer here_ You can: Send the length of the message before the command.
                       Use a special delimiter (e.g., newline) to mark the end.
If not handled, issues like message splitting or concatenation can occur.

3. Describe the general differences between stateful and stateless protocols.

_answer here_ Stateful: The server remembers past interactions (e.g., HTTP with sessions).
              Stateless: Each request is independent; the server doesn’t store context (e.g., UDP).

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_answer here_ We use UDP because:
    Faster communication due to no connection setup.
    Essential for real-time applications (e.g., VoIP, gaming).
    Useful for broadcasting to multiple receivers.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_answer here_ The operating system provides sockets, which allow communication over a network using TCP/UDP protocols through functions like socket(), send(), and recv().