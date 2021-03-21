all:tcp_cli tcp_srv
tcp_cli:tcp_cli.cpp
	g++ -std=c++11 $^ -o $@
tcp_srv:tcp_srv.cpp
	g++ -std=c++11 $^ -o $@
nblock:nblock.c
	gcc $^ -o $@
