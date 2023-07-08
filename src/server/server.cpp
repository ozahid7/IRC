#include "server.hpp"

bool is_it_digits(std::string str){
	for (size_t i = 0; i < str.length(); i++){
		if  (std::isdigit(str[i]))
			return(false);
	}
	return (true);
}

int	parce_port(char *str){
	std::string port = str;
	if (is_it_digits(port))
		return (-1);
	int nb = atoi(port.c_str());
	if (nb < 0 || nb > 65535)
		return (-1);
	return (nb);
}

void Server::create_bind_listen(int port)
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("socket");
		exit(1);
	}
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_server.sin_port = htons(port);
	if (bind(server_socket, (struct sockaddr *)&addr_server, sizeof(addr_server)) < 0)
	{
		perror("bind");
		exit (1);
	}
	if (listen(server_socket, 5) < 0)
	{
		perror("listen");
		exit(1);
	}
}

void Server::accept_new_client()
{
	client_addr_len = sizeof(addr_client);
	client_socket = accept(server_socket, (struct sockaddr *)&addr_client, (socklen_t*)&client_addr_len);
	if (client_socket < 0)
	{
		perror("accept");
		exit (1);
	}
	if (poll_fds.size() < MAXCLIENTS)
	{
		pollfd p;
		p.fd = client_socket;
		p.events = POLLIN;
		poll_fds.push_back(p);
		std::cout<<"new connection from client : "<<inet_ntoa(addr_client.sin_addr)<<"  in port : "<<htons(port)<<std::endl;
		send(client_socket, "wellcome to the server\n", 23, 0);
		//hna fin kan3amer client data t9dar tzid ay haja fhadak paramitrised constructor o tmchi t3amerha flclient
		clients_map.insert(std::make_pair(client_socket, Client(addr_client, client_socket)));
	}
}

Server::Server(char **av)
{
	ClientIter	client_iter;
	port = parce_port(av[1]);
	password = av[2]; 
	if(password.empty()){
		std::cout<<"Please Enter A Valid Password"<<std::endl;
		exit(1);
	}
	if (port == -1)
	{
		std::cout<<"Invalid Port"<<std::endl;
		exit(1);
	}
	create_bind_listen(port);
	pollfd p;
	p.fd = server_socket;
	p.events = POLLIN;
	poll_fds.push_back(p);
	while (1){
		poll_result = poll(poll_fds.data(), poll_fds.size(), -1);
		if (poll_result < 0){
			perror("poll");
			exit(1);
		}
		while (poll_result > 0){
			PollFds tmp = poll_fds;
			for (PollIter it = tmp.begin(); it != tmp.end(); it++){
				if (it->revents & POLLIN && it->fd == server_socket){
					//ila kent kat9aleb fin t9dar tzid data f Client class dkhol lhad lfonction atl9ani mkhalli lk fiha comment
					accept_new_client();
				}else{
					if (it->revents & (POLLHUP | POLL_ERR)){
						//hna fin kanmsa7 lclient mn clientsmap ma3raftch wach at7tajoha fchi 7aja
						printf("Client disconnected\n");
						if (clients_map.size() > 0){
							client_iter = clients_map.find(it->fd);
							clients_map.erase(client_iter);
						}
						for (PollIter pit = poll_fds.begin(); pit < poll_fds.end(); pit++){
							if(it->fd == pit->fd){
								poll_fds.erase(pit);
							}
						}
						close(it->fd);
					}
					else if (it->revents & POLLIN){
						int recv_len = recv(it->fd, buffer, BUFFERSIZE, 0);
						buffer[recv_len] = '\0';
						//ay haja rseltiha  mn lclient atl9aha fhad lbuffer les command dima ayb9aw wjiwkom hna b7all "pass kick" wa majawarahoma
						printf("Received from client : %s", buffer);
						//hna fin t9dar tjawb lclient khdem bhad send li lta7t 3tiha it->fd o kteb lclient li bghiti
						send(it->fd, "Message received\n", 17, 0);
					}
				}
			}
			poll_result--;
		}
	}
	close(server_socket);
}

Server::~Server()
{

}