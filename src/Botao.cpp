#include "Botao.h"

#include <cmath>

// Construtor
// Cria um botão que contém um círculo
Botao::Botao(const float x, const float y, const float r, int grossura_borda, sf::Color cor_borda)
{
	this->circulo.setRadius(r);
	this->circulo.setOrigin(x, y);
	this->circulo.setPosition(2 * x - this->circulo.getRadius(),
							  2 * y - this->circulo.getRadius());
	this->circulo.setOutlineColor(cor_borda);
	this->circulo.setOutlineThickness(grossura_borda);
}

// Detecta se x e y estão sobre este este botão
bool Botao::clicado(const int x, const int y) const
{
	return this->circulo.getRadius() >= std::sqrt(std::pow(this->circulo.getOrigin().x - x, 2) +
												  std::pow(this->circulo.getOrigin().y - y, 2));
}

// Retorna o círculo deste botão
sf::CircleShape &Botao::get_circulo()
{
	return this->circulo;
}

// Retorna a cor do círculo deste botão
sf::Color Botao::get_cor()
{
	return this->circulo.getFillColor();
}

// Redefine a cor do círculo deste botão
void Botao::set_cor(sf::Color cor)
{
	this->circulo.setFillColor(cor);
}