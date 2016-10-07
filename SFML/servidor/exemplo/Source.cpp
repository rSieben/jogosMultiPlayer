#include <SFML/Graphics.hpp>
#include <iostream>
#include "Jogo.h"
#include <thread>

WSAData wsaData;
SOCKET servidorSocket;
SOCKET socket_CONNECTION;
sockaddr_in servidorEndereco;

int main() {
#ifndef __linux__
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	SOCKET principalSocket;
	principalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (principalSocket == SOCKET_ERROR) {
		std::cout << "Ocorreu um erro ao criar socket" << std::endl;
		return 1;
	}

	sockaddr_in enderecoServidor;
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);
	inet_pton(AF_INET, "0.0.0.0", &enderecoServidor.sin_addr);

	int r;
	r = bind(principalSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));
	r = listen(principalSocket, SOMAXCONN);

	SOCKET clienteSocket;
	SOCKET clienteSocket2;
	sockaddr_in enderecoCliente;
	int enderecoClienteTam = sizeof(sockaddr_in);
	clienteSocket = accept(principalSocket, (sockaddr*)&enderecoCliente, &enderecoClienteTam);
	clienteSocket2 = accept(principalSocket, (sockaddr*)&enderecoCliente, &enderecoClienteTam);

	for (;;) {

		char posX,posY;
		char posX2, posY2;

		recv(clienteSocket, &posX, sizeof(posX), 0);
		recv(clienteSocket, &posY, sizeof(posY), 0);
		send(clienteSocket2, &posX, sizeof(posX), 0);
		send(clienteSocket2, &posY, sizeof(posY), 0);

		recv(clienteSocket2, &posX2, sizeof(posX2), 0);
		recv(clienteSocket2, &posY2, sizeof(posY2), 0);
		send(clienteSocket, &posX2, sizeof(posX2), 0);
		send(clienteSocket, &posY2, sizeof(posY2), 0);

	}

	WSACleanup();

}
