To run:-

On one terminal run server as mentioned below:-
$ cd Networking\ Workshop/
$ cd Echo\ Server/
$ gcc sserver.c
$ ./a.out 5000

On another terminal run client as mentioned below:-
$ cd Networking\ Workshop/
$ cd Echo\ Server/
$ gcc sclient.c
$ ./a.out 5000

Note:- To close the server,use ctrl+c.

To get server ip type 
$ ifconfig


To demonstrate in 2 different systems, change the inet_addr to server ip address in both client and server file. Now run both the files same way mentioned above in 2 different systems.
