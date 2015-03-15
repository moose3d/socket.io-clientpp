all:
	g++ -std=c++11 -c -I/usr/include/websocketpp  -I../lib/rapidjson/include/ -Wall socket_io_client.cpp -lboost_system -lboost_thread -lboost_regex -lpthread
	ar rcs libsocketio.a socket_io_client.o
