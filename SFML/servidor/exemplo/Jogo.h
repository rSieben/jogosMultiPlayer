#pragma once
#include <SFML\Graphics.hpp>
#include "Megaman.h"
#include "Mouse.h"

class Jogo
{
private:
	sf::RenderWindow m_janela;
	sf::Time m_tempoPorFrame;
	Megaman m_megaman;
	Mouse m_mouse;

public :
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

			Renderizar();
		}

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
		m_janela.display();
	}
};