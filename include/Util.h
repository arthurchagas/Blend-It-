#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <random>

sf::Color cor_aleatoria_rgb(std::default_random_engine &gerador);
sf::Color cor_aleatoria_rg(std::default_random_engine &gerador, unsigned b);
sf::Color cor_aleatoria_rb(std::default_random_engine &gerador, unsigned g);
sf::Color cor_aleatoria_gb(std::default_random_engine &gerador, unsigned r);

sf::Color misturar(sf::Color a, sf::Color b);
sf::Color inverter(sf::Color a);

sf::CircleShape criar_circulo(float raio, int x_origem, int y_origem, int grossura_borda = 0,
                              sf::Color cor_preencher = sf::Color::White, sf::Color cor_borda = sf::Color::White);

sf::Uint8 escolher_componente_constante(std::default_random_engine &gerador, sf::Color cor,
                                        sf::Color (**gerador_de_cor)(std::default_random_engine &, unsigned));

#endif //UTIL_H