#ifndef UTIL_H
#define UTIL_H

#include "AVL.h"
#include "Botao.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <random>

sf::Color cor_aleatoria_rgb(std::default_random_engine &gerador);
sf::Color cor_aleatoria_rg(std::default_random_engine &gerador, unsigned b);
sf::Color cor_aleatoria_rb(std::default_random_engine &gerador, unsigned g);
sf::Color cor_aleatoria_gb(std::default_random_engine &gerador, unsigned r);

sf::Color misturar(sf::Color a, sf::Color b);
sf::Color inverter(sf::Color a);

sf::CircleShape criar_circulo(float raio, int x_origem, int y_origem, int grossura_borda = 0,
                              sf::Color cor_preencher = sf::Color::White, sf::Color cor_borda = sf::Color::White);
sf::Text criar_texto(sf::String conteudo, sf::Font &fonte, unsigned tamanho, sf::Color cor_preencher = sf::Color::White,
                     int x = 0, int y = 0);
void criar_formas(std::default_random_engine &gerador, sf::CircleShape &atual, sf::CircleShape &alvo,
                  sf::CircleShape &preview, sf::RectangleShape &alvo_background);

sf::Uint8 escolher_componente_constante(std::default_random_engine &gerador, sf::Color cor,
                                        sf::Color (**gerador_de_cor)(std::default_random_engine &, unsigned));

int calcular_pontuacao(sf::Color a, sf::Color b);

void iniciar(std::default_random_engine &gerador, sf::CircleShape &atual, sf::CircleShape &alvo,
             sf::CircleShape &preview, sf::RectangleShape &alvo_background, sf::Font &fonte, sf::Text &alvo_txt,
             sf::Text &pontuacao_txt);
void novo_jogo(std::default_random_engine &gerador, avl_t &arvore, sf::Color cor_base, apontador_t &raiz_atual,
               Botao *&bt, Botao *&bt2, unsigned altura);
void desenhar_jogo(sf::RenderWindow &janela, sf::RectangleShape &alvo_background, sf::Text &alvo_txt, Botao *&bt,
                   Botao *&bt2, sf::Event::MouseMoveEvent event, sf::CircleShape &preview, sf::CircleShape &atual,
                   sf::CircleShape &alvo);
void desenhar_fim_de_jogo(sf::RenderWindow &janela, sf::Text pontuacao_txt, sf::CircleShape &atual,
                          sf::CircleShape &alvo);

#endif //UTIL_H