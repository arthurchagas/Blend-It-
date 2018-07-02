#include "Botao.h"
#include "Util.h"
#include "AVL.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <SFML/Window/Event.hpp>

#include <chrono>
#include <cmath>

int main()
{
    bool fim = false;
    int pontuacao;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Blender");

    std::default_random_engine gerador;
    gerador.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    sf::CircleShape atual(100.f);
    atual.setOutlineThickness(-2);
    atual.setOutlineColor(sf::Color::White);
    atual.setOrigin(400, 300);
    atual.setPosition(2 * atual.getOrigin().x - atual.getRadius(), 2 * atual.getOrigin().y - atual.getRadius());

    sf::CircleShape alvo(50.0f);
    alvo.setOutlineThickness(-2);
    alvo.setOutlineColor(sf::Color::White);
    alvo.setOrigin(750, 50);
    alvo.setPosition(2 * alvo.getOrigin().x - 75, alvo.getOrigin().y + 25);
    alvo.setFillColor(cor_aleatoria_rgb(gerador));

    sf::CircleShape preview(50.0f);
    preview.setOutlineThickness(-2);
    preview.setOutlineColor(sf::Color::White);
    preview.setOrigin(400, 300 - 1.75f * atual.getRadius());
    preview.setPosition(2 * preview.getOrigin().x - preview.getRadius(), 2 * preview.getOrigin().y - preview.getRadius());

    sf::RectangleShape alvo_background({2.25f * alvo.getRadius(), 2.5f * alvo.getRadius()});
    alvo_background.setOrigin(alvo.getOrigin().x - 0.125f * alvo.getRadius(), alvo.getOrigin().y - 0.125f * alvo.getRadius());
    alvo_background.setPosition(2 * alvo_background.getOrigin().x - 75, alvo_background.getOrigin().y);
    alvo_background.setFillColor(sf::Color(255 - alvo.getFillColor().r, 255 - alvo.getFillColor().g, 255 - alvo.getFillColor().b));

    sf::Font fonte;
    fonte.loadFromFile("rsc/arial.ttf");

    sf::Text alvo_txt("Cor-Alvo", fonte, 20);
    alvo_txt.setFillColor(alvo.getFillColor());
    alvo_txt.setPosition(alvo_background.getPosition().x - alvo_background.getOrigin().x,
                         alvo_background.getPosition().y - alvo_background.getOrigin().y);

    sf::Text pontuacao_txt("", fonte, 48);

    avl_t arvore;
    criaArvore(&arvore);

    std::uniform_int_distribution<int> cor_constante_rand(0, 2);
    int cor_constante = cor_constante_rand(gerador);
    unsigned cor_constante_valor;
    sf::Color (*gerador_de_cor)(std::default_random_engine &, unsigned);

    if (cor_constante == 0)
    {
        gerador_de_cor = &cor_aleatoria_gb;
        cor_constante_valor = alvo.getFillColor().r;
    }
    else if (cor_constante == 1)
    {
        gerador_de_cor = &cor_aleatoria_rb;
        cor_constante_valor = alvo.getFillColor().g;
    }
    else
    {
        gerador_de_cor = &cor_aleatoria_rg;
        cor_constante_valor = alvo.getFillColor().b;
    }

    for (int k = 0; k < 256; ++k)
        inserir(&arvore, {(*gerador_de_cor)(gerador, cor_constante_valor)});

    apontador_t raiz_atual = arvore;

    auto bt = new Botao(400 - 2 * atual.getRadius(), 300, 50.0f);
    auto bt2 = new Botao(400 + 2 * atual.getRadius(), 300, 50.0f);

    bt->set_cor(raiz_atual->esquerda->item.cor);
    bt2->set_cor(raiz_atual->direita->item.cor);

    int mousePos[2];

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (!fim)
                {
                    bool bt_clicado = bt->clicado(event.mouseButton.x, event.mouseButton.y);
                    bool bt2_clicado = bt2->clicado(event.mouseButton.x, event.mouseButton.y);

                    if (!bt_clicado && !bt2_clicado)
                        continue;

                    atual.setFillColor(preview.getFillColor());

                    if (bt_clicado)
                        raiz_atual = raiz_atual->esquerda;
                    else
                        raiz_atual = raiz_atual->direita;

                    if (raiz_atual->esquerda == nullptr || raiz_atual->direita == nullptr)
                        fim = true;

                    if (!fim)
                    {
                        bt->set_cor(raiz_atual->esquerda->item.cor);
                        bt2->set_cor(raiz_atual->direita->item.cor);

                        if (bt_clicado)
                            preview.setFillColor(misturar(bt->get_cor(), atual.getFillColor()));
                        else
                            preview.setFillColor(misturar(bt2->get_cor(), atual.getFillColor()));
                    }
                }

                if (fim)
                {
                    pontuacao = static_cast<int>(std::ceil(std::sqrt(2 * 255 * 255)) -
                                std::sqrt(std::pow(alvo.getFillColor().r - atual.getFillColor().r, 2) +
                                          std::pow(alvo.getFillColor().g - atual.getFillColor().g, 2) +
                                          std::pow(alvo.getFillColor().b - atual.getFillColor().b, 2)));
                    pontuacao_txt.setString("Pontuacao: " + std::to_string(pontuacao));
                    pontuacao_txt.setFillColor(alvo.getFillColor());
                }
            }
            if (!fim && event.type == sf::Event::MouseMoved)
            {
                mousePos[0] = event.mouseMove.x;
                mousePos[1] = event.mouseMove.y;

                if (bt->clicado(mousePos[0], mousePos[1]))
                    preview.setFillColor(misturar(bt->get_cor(), atual.getFillColor()));
                else if (bt2->clicado(mousePos[0], mousePos[1]))
                    preview.setFillColor(misturar(bt2->get_cor(), atual.getFillColor()));
            }
        }

        window.clear();
        if (!fim)
        {
            window.draw(alvo_background);
            window.draw(alvo_txt);
            window.draw(bt->get_circulo());
            window.draw(bt2->get_circulo());

            if (bt->clicado(mousePos[0], mousePos[1]) || bt2->clicado(mousePos[0], mousePos[1]))
                window.draw(preview);
        }
        else
        {
            window.draw(pontuacao_txt);
        }

        window.draw(alvo);
        window.draw(atual);
        window.display();
    }

    delete bt;
    delete bt2;
    esvaziarArvore(&arvore);

    return 0;
}