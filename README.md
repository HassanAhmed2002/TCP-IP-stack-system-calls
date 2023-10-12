# TCP-IP-stack-system-calls
Sender/receiver program to transmit “Audio File”
contents over TCP sockets using various TCP/IP stack system calls. It is required to code in GNU C on the
Linux operating system only.
Sender requirements:
The following are the sender’s requirements:
a) Able to open and read the contents of an audio file
b) Create segments and send them separately over TCP
c) Indicate the end of the file to the receiver
d) Close the socket once the complete file is sent
Receiver requirements:
The following are receiver requirements:
a) Able to open a new audio file
b) Receive segments from the sender and write them in a new file
c) Able to identify eof to terminate
d) Close the file and socket as well

