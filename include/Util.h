#ifndef UTIL_H
#define UTIL_H

#include "Util/Cor.h"
#include "AVL.h"
#include "Botao.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <random>
#include <chrono>

sf::CircleShape criar_circulo(float raio, int x_origem, int y_origem, int grossura_borda = 0,
                              sf::Color cor_preencher = sf::Color::White, sf::Color cor_borda = sf::Color::Black);
sf::Text criar_texto(sf::String conteudo, sf::Font &fonte, unsigned tamanho, sf::Color cor_preencher = sf::Color::White,
                     int x = 0, int y = 0);

sf::Uint8 escolher_componente_constante(std::default_random_engine &gerador, sf::Color cor,
                                        sf::Color (**gerador_de_cor)(std::default_random_engine &, unsigned));

std::string pontuacao(sf::Color a, sf::Color b);

void iniciar(std::default_random_engine &gerador, sf::Font &fonte);
void novo_jogo(std::default_random_engine &gerador, avl_t &arvore, sf::Color cor_base, apontador_t &raiz_atual,
               Botao *&bt, Botao *&bt2, unsigned altura);
void desenhar_jogo(sf::RenderWindow &janela, sf::Text &alvo_txt, Botao *&bt, Botao *&bt2, int x_mouse, int y_mouse,
                   sf::CircleShape &preview, sf::CircleShape &atual, sf::CircleShape &alvo);
void desenhar_fim_de_jogo(sf::RenderWindow &janela, sf::Text pontuacao_txt, sf::CircleShape &atual,
                          sf::CircleShape &alvo);

#endif //UTIL_H