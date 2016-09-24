#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class Mouse {

private:
	float m_x, m_y;
	int frameAtual;
	sf::Time m_proximaAtualizacao = sf::seconds(0.2f);
	sf::Sprite m_sprite;
	sf::Texture m_textura;
	sf::RenderWindow* m_janela;
	std::vector<sf::IntRect> m_frames;

public:
	Mouse(sf::RenderWindow* janela)
		: m_janela(janela)
	{
		bool r;
		r = m_textura.loadFromFile("mouse.png");
		m_sprite.setTexture(m_textura);
		m_frames.push_back(sf::IntRect(0,0,17,25));
	}

	void Atualizar(const sf::Time & tempo)
	{

		if (sf::Mouse::getPosition().x <= 800)
			m_x = sf::Mouse::getPosition().x;

		if (sf::Mouse::getPosition().y <= 600)
			m_y = sf::Mouse::getPosition().y;

		m_sprite.setPosition(m_x, m_y);
		
		if (m_proximaAtualizacao <= sf::Time::Zero)
		{
			frameAtual = (frameAtual + 1) % 3;
			m_proximaAtualizacao = sf::seconds(0.2f);
		}
		else
			m_proximaAtualizacao -= tempo;
	}

	void Desenhar()
	{
		m_janela->draw(m_sprite);
	}

};