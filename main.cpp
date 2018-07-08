#include "Botao.h"
#include "Util.h"
#include "AVL.h"

int main()
{
    // Flags que controlam o que será desenhado na tela
    bool fim = false, inicio = true;
    
    // Coordenadas do mouse
    int x_mouse = 0, y_mouse = 0;

    // Gerador de números pseudo-aleatórios global
    std::default_random_engine gerador;

    // Janela de jogo
    sf::RenderWindow janela(sf::VideoMode(800, 600), "Blend It!");

    // Círculos usados para mostrar cores
    sf::CircleShape atual, alvo, preview;

    // Textos utilizados no jogo
    sf::Text alvo_txt, pontuacao_txt;

    // Fonte global utilizada no jogo
    sf::Font fonte;

    // Imagens utilizadas no jogo
    sf::Texture tela_inicio_texture, botao_inicio_texture, tela_final_texture;
    sf::Sprite tela_inicio, botao_inicio, tela_final;

    // AVL de cores utilizada no jogo
    avl_t arvore;

    // Aponta para o nó-cor atual
    apontador_t raiz_atual;

    // Botões utilizados no jogo (nó à esquerda, nó à direita, começar jogo)
    Botao *bt, *bt2, *bt_inicio;

    // Iniciar gerador de números aleatórios, carregar fontes e imagens
    iniciar(gerador, fonte, tela_inicio_texture, tela_inicio, botao_inicio_texture, botao_inicio,
            tela_final_texture, tela_final);

    // Preparar botão de início
    bt_inicio = new Botao(400, 425, 110);
    botao_inicio.setPosition(250, 275);

    // Preparar círculo que mostra cor atual
    atual = criar_circulo(100.0f, 400, 300, -6);
    atual.setPosition(700, 500);

    // Preparar círculo que mostra cor alvo
    alvo = criar_circulo(50.0f, 750, 50, -4, cor_aleatoria_rgb(gerador));
    alvo.setPosition(1425, 75);

    // Preparar círculo que mostra mistura entre cor atual e cor à direita ou esquerda do nó atual
    preview = criar_circulo(50.0f, 400, 125, -4);
    preview.setPosition(750, 200);

    // Preparar textos
    alvo_txt = criar_texto("Cor-Alvo", fonte, 20, alvo.getFillColor(), 668.75f);
    pontuacao_txt = criar_texto("", fonte, 48, alvo.getFillColor(), 480, 455);

    // Preparar um novo jogo
    novo_jogo(gerador, arvore, alvo.getFillColor(), raiz_atual, bt, bt2, 10);

    while (janela.isOpen())
    {
        sf::Event event;
        while (janela.pollEvent(event))
        {
            // Janela foi fechada
            if (event.type == sf::Event::Closed)
                janela.close();

            // Botão esquerdo do mouse foi clicado
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // Botão de ínicio foi clicado
                if (inicio && bt_inicio->clicado(event.mouseButton.x, event.mouseButton.y))
                    inicio = false;
                // Clique na tela de jogo
                else if (!fim)
                {
                    // Checa se um dos botões foi clicado (nó à esquerda ou nó à direita)
                    bool bt_clicado = bt->clicado(event.mouseButton.x, event.mouseButton.y);
                    bool bt2_clicado = bt2->clicado(event.mouseButton.x, event.mouseButton.y);

                    // Nenhum botão foi clicado (clique irrelevante)
                    if (!bt_clicado && !bt2_clicado)
                        continue;

                    // Atualizar cor atual com a mistura entre a cor atual e a cor do botão clicado
                    atual.setFillColor(preview.getFillColor());

                    // Botão à esquerda foi clicado (nó à esquerda)
                    if (bt_clicado)
                        raiz_atual = raiz_atual->esquerda;
                    //Botão à direita foi clicado (nó à direita)
                    else
                        raiz_atual = raiz_atual->direita;

                    // Atingimos um nó folha, o jogo terminou
                    if (raiz_atual->esquerda == nullptr || raiz_atual->direita == nullptr)
                        fim = true;

                    // O jogo ainda não terminou
                    if (!fim)
                    {
                        // Atualizar cores dos círculos à esquerda e à direita
                        bt->set_cor(raiz_atual->esquerda->item.cor);
                        bt2->set_cor(raiz_atual->direita->item.cor);

                        // Atualizar a cor de preview neste mesmo evento
                        // Remove a necessidade de mover o mouse após escolher uma cor para atualizar o preview
                        if (bt_clicado)
                            preview.setFillColor(misturar(bt->get_cor(), atual.getFillColor()));
                        else
                            preview.setFillColor(misturar(bt2->get_cor(), atual.getFillColor()));
                    }
                    // O jogo terminou nesta rodada
                    else
                    {
                        // Calcular pontuação (porcentagem de acerto)
                        pontuacao_txt.setString(pontuacao(alvo.getFillColor(), atual.getFillColor()));

                        // Redimensionar círculo da cor alvo
                        alvo.setRadius(100.0f);
                        alvo.setOutlineThickness(-6);

                        // Mover círculos de cor para as posições corretas da tela de fim de jogo
                        // Estas posições são relativas ao próprio sistema de coordenadas (setOrigin)
                        alvo.setPosition(875, 225);
                        atual.setPosition(875, 475);
                    }
                }
                // Clique na tela de fim de jogo
                // Um clique em qualquer posição nesta dela reinicia o jogo
                else
                {
                    // Reseta as flags
                    inicio = true;
                    fim = false;

                    // Resetar (deletar) botões à esquerda e à direita
                    delete bt;
                    delete bt2;

                    // Reseta a àrvore de cores
                    esvaziarArvore(&arvore);

                    // Escolhe uma nova cor alvo
                    alvo.setFillColor(cor_aleatoria_rgb(gerador));

                    // Reseta a cor atual
                    atual.setFillColor(sf::Color::White);

                    // Redimensionar o círculo da cor alvo para o original
                    alvo.setRadius(50.0f);
                    alvo.setOutlineThickness(-4);

                    // Mover os círculos para as posições originais
                    alvo.setPosition(1425, 75);
                    atual.setPosition(700, 500);

                    // Atualizar a cor dos textos
                    alvo_txt.setFillColor(alvo.getFillColor());
                    pontuacao_txt.setFillColor(alvo.getFillColor());

                    // Preparar um novo jogo
                    novo_jogo(gerador, arvore, alvo.getFillColor(), raiz_atual, bt, bt2, 10);
                }
            }

            // Mouse se moveu
            if (!fim && event.type == sf::Event::MouseMoved)
            {
                // Atualizar a posição do mouse
                x_mouse = event.mouseMove.x;
                y_mouse = event.mouseMove.y;

                // Checa se o mouse está sobre o botão à esquerda ou sobre o botão à direita
                // Atualiza a cor do círculo de preview misturando a cor atual com a cor do círculo que está sob o mouse
                if (bt->clicado(x_mouse, y_mouse))
                    preview.setFillColor(misturar(bt->get_cor(), atual.getFillColor()));
                else if (bt2->clicado(x_mouse, y_mouse))
                    preview.setFillColor(misturar(bt2->get_cor(), atual.getFillColor()));
            }
        }

        // Limpa a janela
        // Pinta o background com o inverso da cor alvo
        janela.clear(inverter(alvo.getFillColor()));

        // Desenha a tela de início
        if (inicio)
            desenhar_inicio(janela, tela_inicio, botao_inicio);
        // Desenha a tela de jogo
        else if (!fim)
            desenhar_jogo(janela, alvo_txt, bt, bt2, x_mouse, y_mouse, preview, atual, alvo);
        // Desenha a tela de fim de jogo
        else
            desenhar_fim_de_jogo(janela, pontuacao_txt, atual, alvo, tela_final);

        // Mostra o que foi desenhado
        janela.display();
    }

    // Libera memória
    delete bt;
    delete bt2;
    esvaziarArvore(&arvore);

    return 0;
}