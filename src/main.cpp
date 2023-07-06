#include "../inc/ircserv.hpp"
#include "httpserver/server.hpp"

int main (int ac, char **av){
	
	if (ac != 3){
		std::cout<<"Invalid Arguments"<<std::endl;
		exit(1);
	}
	Server a(av);
}