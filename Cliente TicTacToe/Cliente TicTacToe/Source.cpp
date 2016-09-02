//arquivo de cliente

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

struct jogada_resposta {
	int comando;
	int x;
	int y;
};

int main() {

	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockaddr_in enderecoServidor;
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);
	inet_pton(AF_INET, "127.0.0.1", &enderecoServidor.sin_addr);

	SOCKET meuSocket;
	meuSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int r;
	r = connect(meuSocket, (sockaddr*)&enderecoServidor, sizeof(sockaddr_in));

	int comando;

	jogada_resposta jr;

	for (;;)
	{

		recv(meuSocket, (char*)&comando, sizeof(comando), 0);
		if (comando == 1)
		{
			std::cout << "Digite X e Y para marcar a posição selecionada" << std::endl;
			std::cout << "Digite X" << std::endl;
			jr.x = getchar();
			std::cout << "Digite Y" << std::endl;
			jr.y = getchar();
			jr.comando = 2;
			send(meuSocket, (char*)&jr, sizeof(jr), 0);
		}
	}

	WSACleanup();

	return 0;
}


