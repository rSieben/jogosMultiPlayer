//arquivo de cliente

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define ENVIAR_JOGADA_REQ 1
#define ENVIAR_JOGADA_RES 2
#define ENVIAR_JOGADA_INVAL 3
#define ENVIAR_VITORIA 10
#define ENVIAR_DERROTA 11
#define ENVIAR_EMPATE 12
#define ENVIAR_AGUARDAR_MSG 13

struct jogada_resposta {
	int comando;
	int x;
	int y;
	char tabuleiro[3][3];
};

int comando;

void mostraTabuleiro(char tab[3][3]) {
	for (int x = 0; x < 3; x++){
		std::cout << "\n" << std::endl;
		for (int y = 0; y < 3; y++)
			std::cout << tab[x][y] << std::endl;
	}
}

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


	jogada_resposta jr;

	for (;;)
	{
		recv(meuSocket, (char*)&comando, sizeof(int), 0);
		switch (comando) {
			// comando de enviar jogada
			case ENVIAR_JOGADA_REQ:
				mostraTabuleiro(jr.tabuleiro);
				std::cout << "\n" << std::endl;
				std::cout << "Digite X e Y para marcar a posição selecionada" << std::endl;
				std::cout << "Digite X" << std::endl;
				std::cin >> jr.x;
				std::cout << "Digite Y" << std::endl;
				std::cin >> jr.y;
				comando = ENVIAR_JOGADA_RES;
				send(meuSocket, (char*)&jr, sizeof(jr), 0);
				break;
			case ENVIAR_JOGADA_INVAL:
				std::cout << "Sua jogada foi inválida tente novamente! \n" << std::endl;

				mostraTabuleiro(jr.tabuleiro);
				std::cout << "\n" << std::endl;

				std::cout << "Digite X e Y para marcar a posição selecionada" << std::endl;
				std::cout << "Digite X" << std::endl;
				std::cin >> jr.x;
				std::cout << "Digite Y" << std::endl;
				std::cin >> jr.y;
				comando = ENVIAR_JOGADA_RES;
				send(meuSocket, (char*)&jr, sizeof(jr), 0);
				break;
			case ENVIAR_AGUARDAR_MSG:
				std::cout << "Aguardando o outro jogador..." << std::endl;
				mostraTabuleiro(jr.tabuleiro);
				break;
			
		}
		
	}

	WSACleanup();

	return 0;
}


