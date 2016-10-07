#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>

class Megaman
{
private:
	float m_x, m_y;
	int m_frameAtual;
	sf::Time m_proximaAtualizacao = sf::seconds(0.2f);
	sf::Sprite m_sprite;
	sf::Texture m_textura;
	sf::RenderWindow* m_janela;
	std::vector<sf::IntRect> m_frames;

	sf::SoundBuffer m_soundBuffer;
	sf::Sound m_sound;

public:

	Megaman(sf::RenderWindow* janela) 
		: m_janela(janela)
	{
		bool r;
		r = m_textura.loadFromFile("megaman.png");
		m_sprite.setTexture(m_textura);

		m_frames.push_back(sf::IntRect(0, 50, 160, 153));
		m_frames.push_back(sf::IntRect(209, 50, 111, 153));
		m_frames.push_back(sf::IntRect(345, 50, 170, 153));
		m_frames.push_back(sf::IntRect(209, 50, 111, 153));
		m_y = 300.f;
		
		r = m_soundBuffer.loadFromFile("explosao.wav");
		m_sound.setBuffer(m_soundBuffer);


		m_frameAtual = m_frames.size() - 1;
	}

	void Atualizar(const sf::Time & tempo)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			m_x += 100.f * tempo.asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			m_x -= 100.f * tempo.asSeconds();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			m_sound.play();

		m_sprite.setPosition(m_x, m_y);

		if (m_proximaAtualizacao <= sf::Time::Zero)
		{
			m_frameAtual = (m_frameAtual + 1) % 3;
			m_proximaAtualizacao = sf::seconds(0.2f);
			m_sprite.setTextureRect(m_frames[m_frameAtual]);
		}
		else
			m_proximaAtualizacao -= tempo;
	}

	void Desenhar()
	{
		m_janela->draw(m_sprite);
	}
};