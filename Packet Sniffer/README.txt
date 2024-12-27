File 1 :- pack.c
It contains basic packet sniffer which just captures ip header fields.
To run:-
$ cd Networking\ Workshop/
$ cd Packet\ Sniffer/
$ gcc pack.c
$ sudo ./a.out

File 2:- fullpack.c
It contains total packet sniffer which captures all the fields upto transport layer. It has modular approach with efficient memory utilization and the output is redirected to a file named log.txt.
To run:-
$ cd Networking\ Workshop/
$ cd Packet\ Sniffer/
$ gcc fullpack.c
$ sudo ./a.out



http://www.binarytides.com/packet-sniffer-code-in-c-using-linux-sockets-bsd-part-2/
