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

int main()
{
    bool fim = false;

    std::default_random_engine gerador;

    sf::RenderWindow janela(sf::VideoMode(800, 600), "Blender");
    sf::CircleShape atual, alvo, preview;
    sf::RectangleShape alvo_background;
    sf::Text alvo_txt, pontuacao_txt;
    sf::Font fonte;

    avl_t arvore;
    apontador_t raiz_atual;
    Botao *bt, *bt2;

    iniciar(gerador, atual, alvo, preview, alvo_background, fonte, alvo_txt, pontuacao_txt);

    novo_jogo(gerador, arvore, alvo.getFillColor(), raiz_atual, bt, bt2, 10);

    while (janela.isOpen())
    {
        sf::Event event;
        while (janela.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                janela.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (!fim)
                {
                    bool bt_clicado = bt->clicado(event.mouseButton);
                    bool bt2_clicado = bt2->clicado(event.mouseButton);

                    if (!bt_clicado && !bt2_clicado)
                        continue;

                    atual.setFillColor(preview.getFillColor());

                    if (bt_clicado)
                        raiz_atual = raiz_atual->esquerda;
                    else
                        raiz_atual = raiz_atual->direita;

                    if (raiz_atual->esquerda == nullptr || raiz_atual->direita == nullptr)
                    {
                        fim = true;
                        pontuacao_txt.setString("Pontuacao: " + std::to_string(calcular_pontuacao(alvo.getFillColor(), atual.getFillColor())));
                        pontuacao_txt.setFillColor(alvo.getFillColor());
                    }

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
                else
                {
                    fim = false;
                    esvaziarArvore(&arvore);
                    criar_formas(gerador, atual, alvo, preview, alvo_background);
                    alvo_txt.setFillColor(alvo.getFillColor());
                    novo_jogo(gerador, arvore, alvo.getFillColor(), raiz_atual, bt, bt2, 10);
                }
            }
            if (!fim && event.type == sf::Event::MouseMoved)
            {
                if (bt->clicado(event.mouseMove))
                    preview.setFillColor(misturar(bt->get_cor(), atual.getFillColor()));
                else if (bt2->clicado(event.mouseMove))
                    preview.setFillColor(misturar(bt2->get_cor(), atual.getFillColor()));
            }
        }

        janela.clear();

        if (!fim)
            desenhar_jogo(janela, alvo_background, alvo_txt, bt, bt2, event.mouseMove, preview, atual, alvo);
        else
            desenhar_fim_de_jogo(janela, pontuacao_txt, atual, alvo);

        janela.display();
    }

    delete bt;
    delete bt2;
    esvaziarArvore(&arvore);

    return 0;
}