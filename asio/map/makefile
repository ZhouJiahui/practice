all:map_server, map_client
map_server:
	g++ -std=c++11 map_server.cc -lboost_system -o map_server
map_client:
	g++ map_client.cc -lboost_system -o map_client
clean:
	rm map_server map_client
.PHONY: clean, map_server, map_client
