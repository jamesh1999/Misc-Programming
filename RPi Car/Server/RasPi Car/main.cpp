#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>



typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;



#define BUFFERSIZE 256
#define C_TO_I(X) (X-'0')

#define FORWARD 18
#define BACKWARD 17
#define LEFT 23
#define RIGHT 24



char cam_port[BUFFERSIZE];
char client_ip[BUFFERSIZE];



void *startStream(void*)
{

	char cmd_buff[BUFFERSIZE] = "mkfifo fifo.";
	strcat(cmd_buff, cam_port);
	system(cmd_buff);

	strncpy(cmd_buff, "cat fifo.", BUFFERSIZE);
	strcat(cmd_buff, cam_port);
	strcat(cmd_buff, " | nc ");
	strcat(cmd_buff, client_ip);
	strcat(cmd_buff, " ");
	strcat(cmd_buff, cam_port);
	strcat(cmd_buff, " &");
	system(cmd_buff);

	std::cout << "Starting stream..." << std::endl;
	strncpy(cmd_buff, "raspivid -o fifo.", BUFFERSIZE);
	strcat(cmd_buff, cam_port);
	strcat(cmd_buff, " -t 0 -n -w 420 -h 300");
	system(cmd_buff);
	std::cout << "Stream ended" << std::endl;

	strncpy(cmd_buff, "rm fifo.", BUFFERSIZE);
	strcat(cmd_buff, cam_port);
	system(cmd_buff);

	return 0;
}

int main()
{

	//Setup the wiringPi library
	if (wiringPiSetupGpio() == -1)
		return 0;

	//Setup GPIO pins
	softPwmCreate(FORWARD, 0, 1024);
	pinMode(BACKWARD, OUTPUT);
	pinMode(LEFT, OUTPUT);
	pinMode(RIGHT, OUTPUT);

	//Create socket
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
		return -1;
	}

	//Bind socket
	if (bind(sock, reinterpret_cast<sockaddr*>(&dest), sizeof(sockaddr_in)) < 0)
	{
		std::cerr << "FAILED: Binding socket" << std::endl;
		close(sock);
		return -1;
	}

	//Listen for client(s)
	std::cout << "Listening on 1337..." << std::endl;
	if (listen(sock, 5) < 0)
	{
		std::cerr << "FAILED: Listening on socket" << std::endl;
		close(sock);
		return -1;
	}

	//Accept connection
	sockaddr_in clientAddress;
	socklen_t clientSize = sizeof(clientAddress);
	sock = accept(sock, reinterpret_cast<sockaddr*>(&clientAddress), &clientSize);
	if (sock < 0)
	{
		std::cerr << "FAILED: Socket connection" << std::endl;
		close(sock);
		return -1;
	}
	std::cout << "Connection Established!" << std::endl;

	//Get IP as char*
	strncpy(client_ip, inet_ntoa(clientAddress.sin_addr), BUFFERSIZE);
	std::cout << "IP: " << client_ip << std::endl;

	std::cout << "Setting up camera..." << std::endl;
	recv(sock, cam_port, BUFFERSIZE, 0);
	std::cout << "On port: " << cam_port << std::endl;
	pthread_t camthread;
	pthread_create(&camthread, NULL, startStream, 0);

	char buffer[BUFFERSIZE];
	int pwr, dir;
	while (true)
	{
		if (recv(sock, buffer, BUFFERSIZE, 0) == -1)
		{
			std::cout << "FAILED: Receiving from socket" << std::endl;
			close(sock);
			return -1;
		}

		if (strncmp(buffer, "exit", 4) == 0)
			break;

		dir = C_TO_I(buffer[0]) - 1;
		pwr = atoi(buffer + 1);

		std::cout << dir << " " << pwr << std::endl;

		//Update output	
		softPwmWrite(FORWARD,  pwr < 0   ? 1024 + pwr : pwr);
		digitalWrite(BACKWARD, pwr < 0   ? HIGH : LOW);
		digitalWrite(LEFT,     dir == -1 ? HIGH : LOW);
		digitalWrite(RIGHT,    dir == 1  ? HIGH : LOW);
	}

	std::cout << "Socket closed" << std::endl;
	close(sock);

	std::cout << "Joining camera thread" << std::endl;
	pthread_join(camthread, NULL);

	return 0;
}