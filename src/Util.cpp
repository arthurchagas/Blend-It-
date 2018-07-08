#include "Util.h"

// Cria um círculo através de um raio, uma coordenada de posição, cor de preenchimento e
//      de borda, e grossura de borda
sf::CircleShape criar_circulo(float raio, int x_origem, int y_origem, int grossura_borda,
                              sf::Color cor_preencher, sf::Color cor_borda)
{
    sf::CircleShape aux(raio);
    aux.setOrigin(x_origem, y_origem);
    aux.setFillColor(cor_preencher);
    aux.setOutlineThickness(grossura_borda);
    aux.setOutlineColor(cor_borda);

    return aux;
}

// Cria um texto a partir de uma fonte, um conteúdo, um tamanho de fonte, uma cor de preenchimento e uma coordenada
//      de posição
sf::Text criar_texto(sf::String conteudo, sf::Font &fonte, unsigned tamanho, sf::Color cor_preencher, int x, int y)
{
    sf::Text aux(conteudo, fonte, tamanho);
    aux.setFillColor(cor_preencher);
    aux.setPosition(x, y);

    return aux;
}

// Escolhe qual vai ser a componente constante (r ou g ou b) deste jogo
// Define também qual gerador de cor será usado (gb ou rb ou rg) neste jogo, dependendo da
//      componente constante definida
sf::Uint8 escolher_componente_constante(std::default_random_engine &gerador, sf::Color cor,
                                        sf::Color (**gerador_de_cor)(std::default_random_engine &, unsigned))
{
    std::uniform_int_distribution<int> cor_constante(0, 2);

    // Sorteia uma cor
    switch (cor_constante(gerador))
    {
    // A componente constante é r
    // O gerador utilizado é gb
    case 0:
        *gerador_de_cor = &cor_aleatoria_gb;
        return cor.r;
    // A compontante constante é g
    // O gerador utilizado é rb
    case 1:
        *gerador_de_cor = &cor_aleatoria_rb;
        return cor.g;
    // A compontante constante é b
    // O gerador utilizado é rg
    default:
        *gerador_de_cor = &cor_aleatoria_rg;
        return cor.b;
    }
}

// Calcula a porcentagem de acerto entre a cor alvo e a cor atual
std::string pontuacao(sf::Color a, sf::Color b)
{
    const double fator = std::sqrt(2 * 255 * 255);
    double pontuacao = fator - distancia(a, b);
    double pontuacao_por_cento = pontuacao / fator * 100;
    return std::to_string(static_cast<int>(std::floor(pontuacao_por_cento))) + "%";
}

// Inicia várias funções necessárias para o funcionamento do jogo
// É chamada uma e apenas uma vez
void iniciar(std::default_random_engine &gerador, sf::Font &fonte, sf::Texture &tela_inicio_texture,
             sf::Sprite &tela_inicio, sf::Texture &botao_inicio_texture, sf::Sprite &botao_inicio,
             sf::Texture &tela_final_texture, sf::Sprite &tela_final)
{
    // Semeia o gerador pseudo-aleatório com o instante atual
    gerador.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    // Carrega a fonte utilizada
    fonte.loadFromFile("rsc/arial.ttf");

    // Carrega a imagem para a tela de início
    tela_inicio_texture.loadFromFile("rsc/TelaInicio.png");
    tela_inicio.setTexture(tela_inicio_texture);

    // Carrega o botão de "jogar" para a tela de início
    botao_inicio_texture.loadFromFile("rsc/Jogar.png");
    botao_inicio.setTexture(botao_inicio_texture);

    // Carrega a imagem para a tela de fim de jogo
    tela_final_texture.loadFromFile("rsc/TelaFinal.png");
    tela_final.setTexture(tela_final_texture);
}

// Prepara um novo jogo
// Deve ser chamada uma e apenas uma vez por novo jogo
void novo_jogo(std::default_random_engine &gerador, avl_t &arvore, sf::Color cor_base, apontador_t &raiz_atual,
               Botao *&bt, Botao *&bt2, unsigned altura)
{
    sf::Uint8 componente_constante;
    sf::Color (*gerador_de_cor)(std::default_random_engine &, unsigned) = nullptr;

    // Cria a árvore de cores
    criaArvore(&arvore);

    // Sorteia a componente constante
    // Define o gerador de cor
    componente_constante = escolher_componente_constante(gerador, cor_base, &gerador_de_cor);

    // Garantir que altura está entre 4 e 15
    if (altura > 15)
        altura = 15;
    
    if (altura < 4)
        altura = 4;

    // Define quantas cores devem ser geradas dependendo da altura desejada
    unsigned limite = static_cast<unsigned>(std::pow(2, altura));

    // Sorteia as cores da árvore
    for (unsigned k = 0; k < limite; ++k)
        // Lidar com cores repetidas
        while(!inserir(&arvore, {(*gerador_de_cor)(gerador, componente_constante)}));

    // Reseta a posição atual na árvore
    raiz_atual = arvore;

    // Cria os botões para escolher entre a cor à esquerda e a cor à direita
    bt = new Botao(200, 300, 50.0f, -4);
    bt2 = new Botao(600, 300, 50.0f, -4);

    // Define a cor dos botões de acordo com a posição inicial da ávore
    bt->set_cor(raiz_atual->esquerda->item.cor);
    bt2->set_cor(raiz_atual->direita->item.cor);
}

// Desenha a tela de jogo
void desenhar_jogo(sf::RenderWindow &janela, sf::Text &alvo_txt, Botao *&bt, Botao *&bt2, int x_mouse, int y_mouse,
                   sf::CircleShape &preview, sf::CircleShape &atual, sf::CircleShape &alvo)
{
    janela.draw(alvo_txt);
    janela.draw(bt->get_circulo());
    janela.draw(bt2->get_circulo());

    // Desenha o preview se o mouse estiver sobre um dos botões de escolha de cor
    if (bt->clicado(x_mouse, y_mouse) || bt2->clicado(x_mouse, y_mouse))
        janela.draw(preview);

    janela.draw(alvo);
    janela.draw(atual);
}

// Desenha a tela de fim de jogo
void desenhar_fim_de_jogo(sf::RenderWindow &janela, sf::Text pontuacao_txt, sf::CircleShape &atual,
                          sf::CircleShape &alvo, sf::Sprite &tela_final)
{
    janela.draw(tela_final);
    janela.draw(pontuacao_txt);
    janela.draw(alvo);
    janela.draw(atual);
}

// Desenha a tela de início de jogo
void desenhar_inicio(sf::RenderWindow &janela, sf::Sprite &tela_inicio, sf::Sprite &botao_inicio)
{
    janela.draw(tela_inicio);
    janela.draw(botao_inicio);
}