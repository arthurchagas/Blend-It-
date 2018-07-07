#include "Botao.h"
#include "Util.h"
#include "AVL.h"

int main()
{
    bool fim = false;
    int x_mouse, y_mouse;

    std::default_random_engine gerador;

    sf::RenderWindow janela(sf::VideoMode(800, 600), "Blender");
    sf::CircleShape atual, alvo, preview;
    sf::RectangleShape alvo_background;
    sf::Text alvo_txt, pontuacao_txt;
    sf::Font fonte;

    avl_t arvore;
    apontador_t raiz_atual;
    Botao *bt, *bt2;

    iniciar(gerador, fonte);

    atual = criar_circulo(100.0f, 400, 300, -2);
    atual.setPosition(700, 500);

    alvo = criar_circulo(50.0f, 750, 50, -2, cor_aleatoria_rgb(gerador));
    alvo.setPosition(1425, 75);

    preview = criar_circulo(50.0f, 400, 125, -2);
    preview.setPosition(750, 200);

    alvo_background.setSize({112.5f, 125});
    alvo_background.setOrigin(743.75f, 43.75f);
    alvo_background.setPosition(1412.5f, 43.75f);
    alvo_background.setFillColor(inverter(alvo.getFillColor()));

    alvo_txt = criar_texto("Cor-Alvo", fonte, 20, alvo.getFillColor(), 668.75f);
    pontuacao_txt = criar_texto("", fonte, 48);

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
                    {
                        fim = true;
                        pontuacao_txt.setString(pontuacao(alvo.getFillColor(), atual.getFillColor()));
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
                    delete bt;
                    delete bt2;
                    esvaziarArvore(&arvore);

                    alvo.setFillColor(cor_aleatoria_rgb(gerador));
                    atual.setFillColor(sf::Color::White);
                    alvo_txt.setFillColor(alvo.getFillColor());
                    alvo_background.setFillColor(inverter(alvo.getFillColor()));

                    novo_jogo(gerador, arvore, alvo.getFillColor(), raiz_atual, bt, bt2, 10);
                }
            }
            if (!fim && event.type == sf::Event::MouseMoved)
            {
                x_mouse = event.mouseMove.x;
                y_mouse = event.mouseMove.y;

                if (bt->clicado(x_mouse, y_mouse))
                    preview.setFillColor(misturar(bt->get_cor(), atual.getFillColor()));
                else if (bt2->clicado(x_mouse, y_mouse))
                    preview.setFillColor(misturar(bt2->get_cor(), atual.getFillColor()));
            }
        }

        janela.clear();

        if (!fim)
            desenhar_jogo(janela, alvo_background, alvo_txt, bt, bt2, x_mouse, y_mouse, preview, atual, alvo);
        else
            desenhar_fim_de_jogo(janela, pontuacao_txt, atual, alvo);

        janela.display();
    }

    delete bt;
    delete bt2;
    esvaziarArvore(&arvore);

    return 0;
}