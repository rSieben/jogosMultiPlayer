// arquivo de servidor

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

struct jogada_resposta {
	int comando;
	int x;
	int y;
};

int player = 1;

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
	sockaddr_in enderecoCliente;
	int enderecoClienteTam = sizeof(sockaddr_in);

	SOCKET clientSocket1 = accept(principalSocket, (sockaddr*)&enderecoCliente, &enderecoClienteTam);
	SOCKET clientSocket2 = accept(principalSocket, (sockaddr*)&enderecoCliente, &enderecoClienteTam);

	int comando = 1;
	jogada_resposta jr;

	for (;;)
	{
		if (player == 1)
		{
			send(clientSocket1, (char*)&comando, sizeof(comando), 0);
			recv(clientSocket1, (char*)&jr, sizeof(jr), 0);
		}
		else {
			send(clientSocket2, (char*)&comando, sizeof(comando), 0);
			recv(clientSocket2, (char*)&jr, sizeof(jr), 0);
		}

	}



	WSACleanup();

}


