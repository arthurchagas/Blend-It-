#ifndef BOTAO_H
#define BOTAO_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>

class Botao {
public:
	Botao(float x, float y, float r, int grossura_borda = 0, sf::Color cor_borda = sf::Color::Black);

	bool clicado(int x, int y) const;

	sf::Color get_cor();
	sf::CircleShape& get_circulo();

	void set_cor(sf::Color cor);

private:
	sf::CircleShape circulo;
};
#endif //BOTAO_H