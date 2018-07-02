#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics/Color.hpp>
#include <random>

sf::Color cor_aleatoria_rgb(std::default_random_engine &gerador);
sf::Color cor_aleatoria_rg(std::default_random_engine &gerador, unsigned b);
sf::Color cor_aleatoria_rb(std::default_random_engine &gerador, unsigned g);
sf::Color cor_aleatoria_gb(std::default_random_engine &gerador, unsigned r);

sf::Color misturar(sf::Color a, sf::Color b);

#endif //UTIL_H