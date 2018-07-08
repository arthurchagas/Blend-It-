#ifndef BOTAO_H
#define BOTAO_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>

class Botao {
public:
	// Construtor
	Botao(float x, float y, float r, int grossura_borda = 0, sf::Color cor_borda = sf::Color::Black);

	// Detecta se x e y estão sobre este este botão
	bool clicado(int x, int y) const;

	// Retorna o círculo deste botão
	sf::CircleShape& get_circulo();

	// Retorna a cor do círculo deste botão
	sf::Color get_cor();

	// Redefine a cor do círculo deste botão
	void set_cor(sf::Color cor);

private:
	sf::CircleShape circulo;
};
#endif //BOTAO_H