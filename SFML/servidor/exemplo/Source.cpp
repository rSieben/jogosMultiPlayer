#include <SFML/Graphics.hpp>
#include <iostream>
#include "Jogo.h"

int main()
{

	sf::Window w;
	

	Jogo jogo(800, 600, "O jogo", 60.f);
	jogo.Rodar();
	return 0;
}