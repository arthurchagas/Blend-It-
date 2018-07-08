#ifndef UTIL_COR_H
#define UTIL_COR_H

#include <random>

#include <SFML/Graphics/Color.hpp>

/* Geradores de cor aleatória */
sf::Color cor_aleatoria_rgb(std::default_random_engine &gerador);
sf::Color cor_aleatoria_rg(std::default_random_engine &gerador, unsigned b);
sf::Color cor_aleatoria_rb(std::default_random_engine &gerador, unsigned g);
sf::Color cor_aleatoria_gb(std::default_random_engine &gerador, unsigned r);

/* Manipuladores de cor */
sf::Color misturar(sf::Color a, sf::Color b);
sf::Color inverter(sf::Color a);
double distancia(sf::Color a, sf::Color b);

#endif //UTIL_COR_H