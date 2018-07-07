#include "Botao.h"

#include <cmath>

Botao::Botao(const float x, const float y, const float r)
{
	this->circulo.setRadius(r);
	this->circulo.setOrigin(x, y);
	this->circulo.setPosition(2 * x - this->circulo.getRadius(),
							  2 * y - this->circulo.getRadius());
	this->circulo.setOutlineColor(sf::Color::White);
	this->circulo.setOutlineThickness(-2);
}

bool Botao::clicado(const int x, const int y) const
{
	return this->circulo.getRadius() >= std::sqrt(std::pow(this->circulo.getOrigin().x - x, 2) +
												  std::pow(this->circulo.getOrigin().y - y, 2));
}

bool Botao::clicado(sf::Event::MouseMoveEvent event) const
{
	return this->clicado(event.x, event.y);
}

bool Botao::clicado(sf::Event::MouseButtonEvent event) const
{
	return this->clicado(event.x, event.y);
}

sf::CircleShape &Botao::get_circulo()
{
	return this->circulo;
}

sf::Color Botao::get_cor()
{
	return this->circulo.getFillColor();
}

void Botao::set_cor(sf::Color cor)
{
	this->circulo.setFillColor(cor);
}