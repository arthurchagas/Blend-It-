#include "Util.h"

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

sf::Text criar_texto(sf::String conteudo, sf::Font &fonte, unsigned tamanho, sf::Color cor_preencher, int x, int y)
{
    sf::Text aux(conteudo, fonte, tamanho);
    aux.setFillColor(cor_preencher);
    aux.setPosition(x, y);

    return aux;
}

sf::Uint8 escolher_componente_constante(std::default_random_engine &gerador, sf::Color cor,
                                        sf::Color (**gerador_de_cor)(std::default_random_engine &, unsigned))
{
    std::uniform_int_distribution<int> cor_constante(0, 2);

    switch (cor_constante(gerador))
    {
    case 0:
        *gerador_de_cor = &cor_aleatoria_gb;
        return cor.r;
    case 1:
        *gerador_de_cor = &cor_aleatoria_rb;
        return cor.g;
    default:
        *gerador_de_cor = &cor_aleatoria_rg;
        return cor.b;
    }
}

std::string pontuacao(sf::Color a, sf::Color b)
{
    const double fator = std::sqrt(2 * 255 * 255);
    double pontuacao = fator - distancia(a, b);
    double pontuacao_por_cento = pontuacao / fator * 100;
    return std::to_string(static_cast<int>(std::floor(pontuacao_por_cento))) + "%";
}

void iniciar(std::default_random_engine &gerador, sf::Font &fonte, sf::Texture &tela_inicio_texture,
             sf::Sprite &tela_inicio, sf::Texture &botao_inicio_texture, sf::Sprite &botao_inicio,
             sf::Texture &tela_final_texture, sf::Sprite &tela_final)
{
    gerador.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    fonte.loadFromFile("rsc/arial.ttf");

    tela_inicio_texture.loadFromFile("rsc/TelaInicio.png");
    tela_inicio.setTexture(tela_inicio_texture);

    botao_inicio_texture.loadFromFile("rsc/Jogar.png");
    botao_inicio.setTexture(botao_inicio_texture);

    tela_final_texture.loadFromFile("rsc/TelaFinal.png");
    tela_final.setTexture(tela_final_texture);
}

void novo_jogo(std::default_random_engine &gerador, avl_t &arvore, sf::Color cor_base, apontador_t &raiz_atual,
               Botao *&bt, Botao *&bt2, unsigned altura)
{
    sf::Uint8 componente_constante;
    sf::Color (*gerador_de_cor)(std::default_random_engine &, unsigned) = nullptr;

    criaArvore(&arvore);
    componente_constante = escolher_componente_constante(gerador, cor_base, &gerador_de_cor);

    unsigned limite = static_cast<unsigned>(std::pow(2, altura));

    for (unsigned k = 0; k < limite; ++k)
        inserir(&arvore, {(*gerador_de_cor)(gerador, componente_constante)});

    raiz_atual = arvore;

    bt = new Botao(200, 300, 50.0f, -4);
    bt2 = new Botao(600, 300, 50.0f, -4);

    bt->set_cor(raiz_atual->esquerda->item.cor);
    bt2->set_cor(raiz_atual->direita->item.cor);
}

void desenhar_jogo(sf::RenderWindow &janela, sf::Text &alvo_txt, Botao *&bt, Botao *&bt2, int x_mouse, int y_mouse,
                   sf::CircleShape &preview, sf::CircleShape &atual, sf::CircleShape &alvo)
{
    janela.draw(alvo_txt);
    janela.draw(bt->get_circulo());
    janela.draw(bt2->get_circulo());

    if (bt->clicado(x_mouse, y_mouse) || bt2->clicado(x_mouse, y_mouse))
        janela.draw(preview);

    janela.draw(alvo);
    janela.draw(atual);
}

void desenhar_fim_de_jogo(sf::RenderWindow &janela, sf::Text pontuacao_txt, sf::CircleShape &atual,
                          sf::CircleShape &alvo, sf::Sprite &tela_final)
{
    janela.draw(tela_final);
    janela.draw(pontuacao_txt);
    janela.draw(alvo);
    janela.draw(atual);
}

void desenhar_inicio(sf::RenderWindow &janela, sf::Sprite &tela_inicio, sf::Sprite &botao_inicio)
{
    janela.draw(tela_inicio);
    janela.draw(botao_inicio);
}