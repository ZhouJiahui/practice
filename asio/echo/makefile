all: async_echo_server blocking_echo_server
async_echo_server:
	g++ async_echo_server.cc -lboost_system -o async_echo_server
blocking_echo_server:
	g++ blocking_echo_server.cc -lboost_system -lboost_thread -o blocking_echo_server
clean:
	rm blocking_echo_server async_echo_server
.PHONY: clean
