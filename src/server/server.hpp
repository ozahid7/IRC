#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserv.hpp"
#include "client.hpp"
#include <netinet/in.h>		/* in_addr */


#define BUFFERSIZE	1024
#define MAXCLIENTS	1000

class Server{
public:
	Server(char **av);
	~Server();
	typedef std::vector<pollfd>::iterator			PollIter;
	typedef std::vector<pollfd> 					PollFds;
	typedef std::map<int, Client>::iterator			ClientIter;
	typedef std::map<int, Client>					ClientMap;
	typedef std::vector<std::string>				VecStr;
	typedef std::vector<std::string>::iterator		VecIter;
	

	void		create_bind_listen(int port);
	void		accept_new_client();
	struct in_addr get_clientip(int fd);
	void		read_client_data(PollIter it);
	int			get_client_info(int fd);
	void		set_pass_and_port(char **av);

private:
	int			server_socket;
	int			client_socket;
	int			poll_result;
	int			port;
	int			client_addr_len;
	std::string	password;
	char 		buffer[BUFFERSIZE];
	std::string	client_msg;
	ClientMap	clients_map;
	PollFds 	poll_fds;
	sockaddr_in addr_server;
	sockaddr_in addr_client;
	VecStr		nick_names;
};
int	parce_port(char *str);
bool is_it_digits(std::string str);
std::vector<std::string> split(std::string str, char c);
std::string trim_spaces(std::string str);

#endif