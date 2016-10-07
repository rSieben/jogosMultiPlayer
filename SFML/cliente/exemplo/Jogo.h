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
	Mouse e_mouse;

public :
	Jogo(int largura,
		 int altura,
		 const char* titulo,
		 float frames)
	: m_tempoPorFrame(sf::seconds(1.0f / frames)),
	  m_janela(sf::VideoMode(largura, altura), titulo),
	  m_megaman(&m_janela),
	  m_mouse(&m_janela),
	  e_mouse(&m_janela)
	{
		
	}
	
	void ProcessarEventos()
	{
		sf::Event evento;
		while (m_janela.pollEvent(evento))
		{
			switch (evento.type)
			{
				case sf::Event::Closed:
					m_janela.close();
					break;
			}
		}
	}

	void Rodar() 
	{
		sf::Clock relogio;
		sf::Time tempoDesdeUltimaAtualizacao = sf::Time::Zero;
		m_janela.setMouseCursorVisible(false);
			

		WSAData wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		sockaddr_in enderecoServidor;
		enderecoServidor.sin_family = AF_INET;
		enderecoServidor.sin_port = htons(9999);
		inet_pton(AF_INET, "127.0.0.1", &enderecoServidor.sin_addr);
		//inet_pton(AF_INET, "10.96.25.159", &enderecoServidor.sin_addr);

		SOCKET meuSocket;
		meuSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		e_mouse.SetSprite("mouse2.png");
		
		int r;
		r = connect(meuSocket, (sockaddr*)&enderecoServidor, sizeof(sockaddr_in));

		while (m_janela.isOpen())
		{
			ProcessarEventos();
			tempoDesdeUltimaAtualizacao += relogio.restart();

			while (tempoDesdeUltimaAtualizacao > m_tempoPorFrame)
			{
				tempoDesdeUltimaAtualizacao -= m_tempoPorFrame;
				ProcessarEventos();
				Atualizar(m_tempoPorFrame);
			}

			char posX;
			char posY;

			posX = m_mouse.GetPositionX();
			posY = m_mouse.GetPositionY();

			send(meuSocket, &posX, sizeof(posX), 0);
			send(meuSocket, &posY, sizeof(posY), 0);
			
			recv(meuSocket, &posX, sizeof(posX), 0);
			recv(meuSocket, &posY, sizeof(posY), 0); 
			
			e_mouse.SetPosition(posX, posY);

			Renderizar();
		}

		WSACleanup();
	}

	void Atualizar(const sf::Time& tempo)
	{
		m_megaman.Atualizar(tempo);
		m_mouse.Atualizar(tempo);
	}

	void Renderizar()
	{
		m_janela.clear();
		m_megaman.Desenhar();
		m_mouse.Desenhar();
		e_mouse.Desenhar();
		m_janela.display();
	}
};