all: cli sev1 netcli netsev sev2

cli: client.cpp function.cpp
	g++ -o client client.cpp function.cpp -I. -pthread

sev1: server1.cpp function.cpp
	g++ -o server1 server1.cpp function.cpp -I. -pthread

netcli: netdclient.cpp function.cpp
	g++ -o netdclient netdclient.cpp function.cpp -I.

netsev: netdsrv.cpp function.cpp
	g++ -o netdsrv netdsrv.cpp function.cpp -I. -pthread

sev2: server2.cpp function.cpp
	g++ -o server2 server2.cpp function.cpp -I.

clean:
	rm *.o