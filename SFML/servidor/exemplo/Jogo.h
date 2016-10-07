#pragma once
#include <SFML\Graphics.hpp>
#include "Megaman.h"
#include "Mouse.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

class Jogo
{

private:
	sf::RenderWindow m_janela;
	sf::Time m_tempoPorFrame;
	Megaman m_megaman;
	Mouse m_mouse;

public :
	WSAData wsaData;
	SOCKET servidorSocket;
	SOCKET socket_CONNECTION;
	sockaddr_in servidorEndereco;
	Jogo(int largura,
		 int altura,
		 const char* titulo,
		 float frames)
	: m_tempoPorFrame(sf::seconds(1.0f / frames)),
	  m_janela(sf::VideoMode(largura, altura), titulo),
	  m_megaman(&m_janela),
	  m_mouse(&m_janela)
	{
		
	}

	void Rodar() 
	{
		sf::Clock relogio;
		sf::Time tempoDesdeUltimaAtualizacao = sf::Time::Zero;
		m_janela.setMouseCursorVisible(false);
			
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		socket_CONNECTION = socket(AF_INET, SOCK_DGRAM, NULL);
		servidorSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		servidorEndereco.sin_family = AF_INET;
		servidorEndereco.sin_port = htons(9999);
		InetPton(AF_INET, "0.0.0.0", &servidorEndereco.sin_addr.s_addr);

		bind(servidorSocket, (SOCKADDR*)&servidorEndereco, sizeof(sockaddr_in));
		listen(servidorSocket, SOMAXCONN);

		for (;;) {
			std::cout << "Esperando conexao" << std::endl;
			char msg[100] = "mouse x e y";
			int servidorEnderecoSize = sizeof(servidorEndereco);
			if (socket_CONNECTION = accept(servidorSocket, (SOCKADDR*)&servidorEndereco, &servidorEnderecoSize)) {
				send(socket_CONNECTION, msg, sizeof(msg), NULL);
			}
		}
	}

};