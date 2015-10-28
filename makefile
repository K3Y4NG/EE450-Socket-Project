compile : client.o serverA.o serverB.o serverC.o serverD.o

client.o :
	gcc -o client client.c -lsocket -lnsl -lresolv

serverA.o :
	gcc -o serverA serverA.c -lsocket -lnsl -lresolv

serverB.o :
	gcc -o serverB serverB.c -lsocket -lnsl -lresolv

serverC.o :
	gcc -o serverC serverC.c -lsocket -lnsl -lresolv

serverD.o :
	gcc -o serverD serverD.c -lsocket -lnsl -lresolv
