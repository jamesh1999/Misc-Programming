#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>



typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;



#define BUFFERSIZE 32

#define FORWARD 1
#define BACKWARD 0
#define LEFT 4
#define RIGHT 5



char cam_port[BUFFERSIZE];
char client_ip[BUFFERSIZE];



void *startStream(void*)
{
	system("mkfifo fifo.13371");
	system("cat fifo.13371 | nc 192.168.1.99 13371 &");
	system("raspivid -o fifo.13371 -t 0 -n -w 420 -h 300");
	system("rm fifo.13371");
}

int main()
{
	//Setup the wiringPi library
	if (wiringPiSetup() == -1)
		return 0;

	//Setup GPIO pins
	softPwmCreate(FORWARD, 0, 1024);
	softPwmCreate(BACKWARD, 0, 1024);
	pinMode(LEFT, OUTPUT);
	pinMode(RIGHT, OUTPUT);

	int sock;
	sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_port = htons(1337);
	dest.sin_addr.s_addr = INADDR_ANY;

	//Connect socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0)
	{
		std::cerr << "FAILED: Socket creation" << std::endl;
		return 0;
	}
	if (bind(sock, (sockaddr*)&dest, sizeof(sockaddr_in)) < 0)
	{
		std::cerr << "FAILED: Binding socket" << std::endl;
		close(sock);
		return 0;
	}
	std::cout << "Listening on 1337..." << std::endl;
	if (listen(sock, 5) < 0)
	{
		std::cerr << "FAILED: Listening on socket" << std::endl;
		close(sock);
		return 0;
	}
	sockaddr_in clientAddress;
	socklen_t clientSize = sizeof(clientAddress);
	sock = accept(sock, (sockaddr*)&clientAddress, &clientSize);
	if (sock < 0)
	{
		std::cerr << "FAILED: Socket connection" << std::endl;
		close(sock);
		return 0;
	}
	std::cout << "Connection Established!" << std::endl;

	client_ip = inet_ntop(client.sin_addr.s_addr);
	std::cout << "IP: " << client_ip << std::endl;

	std::cout << "Setting up camera" << std::endl;
	recv(sock, cam_port, BUFFERSIZE, 0);
	std::cout << "On port: " << cam_port << std::endl;
	pthread_t camthread;
	pthread_create(&camthread, NULL, startStream, 0);

	bool cont = true;
	char buffer[BUFFERSIZE];
	int pwr, dir;
	while (cont)
	{
		if (recv(sock, buffer, BUFFERSIZE, 0) == -1)
		{
			std::cout << "FAILED: Receiving from socket" << std::endl;
			close(sock);
			return 0;
		}

		if (strncmp(buffer, "exit", 4) == 0)
		{
			cont = false;
			dir = pwr = 0;
		}
		else
		{
			dir = (buffer[0] - '0') - 1;
			pwr = (buffer[1] == '-' ? -1 : 1) * (((buffer[2] - '0') * 1000) + ((buffer[3] - '0') * 100) + ((buffer[4] - '0') * 10) + (buffer[5] - '0'));
		}

		std::cout << dir << " " << pwr << std::endl;

		//Update output
		softPwmWrite(FORWARD, (pwr > 0 ? pwr : 0));
		softPwmWrite(BACKWARD, (pwr < 0 ? -pwr : 0));
		digitalWrite(LEFT, (dir == -1 ? HIGH : LOW));
		digitalWrite(RIGHT, (dir == 1 ? HIGH : LOW));
	}

	std::cout << "Socket closed" << std::endl;
	close(sock);

	std::cout << "Joining camera thread" << std::endl;
	pthread_join(camthread, NULL);

	return 0;
}