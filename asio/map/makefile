all: test_map_server map_server map_client_main map_client.o common.o
test_map_server: map_client.o
	g++ -std=c++11 map_client.o test_map_server.cc -lboost_system -lpthread -lgtest -o test_map_server
map_server: common.o
	g++ -std=c++11 common.o map_server.cc -lboost_system -o map_server
map_client_main: map_client.o
	g++ -std=c++11 common.o map_client.o map_client_main.cc -lboost_system -lpthread -o map_client_main
map_client.o: common.o
	g++ -std=c++11 -c common.o map_client.cc -lboost_system -lpthread -o map_client.o
common.o:
	g++ -std=c++11 -c common.cc -o common.o
clean:
	rm test_map_server map_server map_client_main map_client.o common.o
.PHONY: clean test_map_server map_server map_client common.o
