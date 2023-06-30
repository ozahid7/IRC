#include "client.hpp"

Client::Client()
{
	addrlen = sizeof(address);
}

int Client::start_client(){
	char av[10000];
	char buffer[30000] = {0};
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0){
		perror("socket");
		return (-1);
	}
	memset(&address, '\0', sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	if (inet_pton(AF_INET, "127.0.0.1",&address.sin_addr) <=  0)
	{
		perror("inet_pton");
		return (-1);
	}
	if (connect(client_fd, (struct sockaddr *)&address, addrlen) < 0)
	{
		perror("connect");
		return (-1);
	}
	std::string str;
	while (1){
		std::cin >> str;
		send (client_fd, str.c_str(), 10000, 2);
		std::cout<<"Client Sent : "<<str<<std::endl;
		int readresult = read(client_fd, buffer, 30000);
		std::cout<<buffer<<std::endl;
		str.clear();
	}
	return (0);
}


int main (){
	Client c;
	c.start_client();
}