// arquivo de servidor

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <random>

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
	char tabuleiro[3][3] = {'a','a','a','a','a','a','a','a','a'};
};

int comando;
int atual = 1;
bool game = true;

bool verificaVitoria(char tabuleiro[3][3],int player) {
	char letra;

	if (player == 1)
		letra = 'X';
	else
		letra = 'O';


	// verifica o tabuleiro se houve vitória ou empate
	// [0][0], [0][1], [0][2]
	// [1][0], [1][1], [1][2]
	// [2][0], [2][1], [2][2]

	if ((tabuleiro[0][0] == letra && tabuleiro[0][1] == letra && tabuleiro[0][2] == letra) || // linha topo
		(tabuleiro[1][0] == letra && tabuleiro[1][1] == letra && tabuleiro[1][2] == letra) || // linha meio
		(tabuleiro[1][0] == letra && tabuleiro[1][1] == letra && tabuleiro[1][2] == letra) || // linha baixo
		(tabuleiro[0][0] == letra && tabuleiro[1][1] == letra && tabuleiro[2][2] == letra) || // diagonal esquerda top direita bot
		(tabuleiro[2][0] == letra && tabuleiro[1][1] == letra && tabuleiro[0][2] == letra) || // diagonal esquerda bot direita top
		(tabuleiro[0][0] == letra && tabuleiro[1][0] == letra && tabuleiro[2][0] == letra) || // coluna esquerda
		(tabuleiro[0][1] == letra && tabuleiro[1][1] == letra && tabuleiro[2][1] == letra) || // coluna meio
		(tabuleiro[0][2] == letra && tabuleiro[1][2] == letra && tabuleiro[2][2] == letra))	  // coluna topo
	{
		return true;
	} else {
		return false;
	}

}

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

	jogada_resposta jr;
	SOCKET player;

	while (game) {

		comando = 1;

		if (atual == 1)	
			player = clientSocket1;
		else 
			player = clientSocket2;

		bool valido = false;

		while (!valido) {
			send(player, (char*)&comando, sizeof(comando), 0);
			recv(player, (char*)&jr, sizeof(jr), 0);

			jr.x = jr.x - 1;
			jr.y = jr.y - 1;

			// verficar e informar se jogada e valida
			if ((jr.x >= 0 && jr.y <= 2)) {
				//atualiza matriz
				jr.tabuleiro[jr.x][jr.y] = atual == 0 ? 'X' : 'O';
				valido = true;
			}
			else {
				comando = ENVIAR_JOGADA_INVAL;
			}
		}

		// verifico se houve vitória
		if (verificaVitoria(jr.tabuleiro, player))
		{
			comando = ENVIAR_VITORIA;
			send(player, (char*)&comando, sizeof(comando), 0);
			recv(player, (char*)&jr, sizeof(jr), 0);

			if (atual == 1)
				player = clientSocket1;
			else
				player = clientSocket2;

			comando = ENVIAR_DERROTA;
			send(player, (char*)&comando, sizeof(comando), 0);
			recv(player, (char*)&jr, sizeof(jr), 0);

			game = false;

		}

		atual = !atual;

	}

	WSACleanup();

}


