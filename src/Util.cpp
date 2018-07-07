#include "Util.h"

#include <chrono>

std::uniform_int_distribution<int> rgb(0, 255);

sf::Color cor_aleatoria_rgb(std::default_random_engine &gerador)
{
    sf::Color aux;

    aux.r = rgb(gerador);
    aux.g = rgb(gerador);
    aux.b = rgb(gerador);

    return aux;
}

sf::Color cor_aleatoria_rg(std::default_random_engine &gerador, unsigned b)
{
    sf::Color aux;

    aux.r = rgb(gerador);
    aux.g = rgb(gerador);
    aux.b = b;

    return aux;
}

sf::Color cor_aleatoria_rb(std::default_random_engine &gerador, unsigned g)
{
    sf::Color aux;

    aux.r = rgb(gerador);
    aux.g = g;
    aux.b = rgb(gerador);

    return aux;
}

sf::Color cor_aleatoria_gb(std::default_random_engine &gerador, unsigned r)
{
    sf::Color aux;

    aux.r = r;
    aux.g = rgb(gerador);
    aux.b = rgb(gerador);

    return aux;
}

sf::Color misturar(sf::Color a, sf::Color b)
{
    return sf::Color(static_cast<sf::Uint8>(a.r / 2.0f + b.r / 2.0f),
                     static_cast<sf::Uint8>(a.g / 2.0f + b.g / 2.0f),
                     static_cast<sf::Uint8>(a.b / 2.0f + b.b / 2.0f));
}

sf::Color inverter(sf::Color a)
{
    return sf::Color(static_cast<sf::Uint8>(255 - a.r),
                     static_cast<sf::Uint8>(255 - a.g),
                     static_cast<sf::Uint8>(255 - a.b));
}

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

void criar_formas(std::default_random_engine &gerador, sf::CircleShape &atual, sf::CircleShape &alvo,
                  sf::CircleShape &preview, sf::RectangleShape &alvo_background)
{
    atual = criar_circulo(100.0f, 400, 300, -2);
    atual.setPosition(2 * atual.getOrigin().x - atual.getRadius(), 2 * atual.getOrigin().y - atual.getRadius());

    alvo = criar_circulo(50.0f, 750, 50, -2, cor_aleatoria_rgb(gerador));
    alvo.setPosition(2 * alvo.getOrigin().x - 75, alvo.getOrigin().y + 25);

    preview = criar_circulo(50.0f, 400, 300 - 1.75f * atual.getRadius(), -2);
    preview.setPosition(2 * preview.getOrigin().x - preview.getRadius(), 2 * preview.getOrigin().y - preview.getRadius());

    alvo_background.setSize({2.25f * alvo.getRadius(), 2.5f * alvo.getRadius()});
    alvo_background.setOrigin(alvo.getOrigin().x - 0.125f * alvo.getRadius(), alvo.getOrigin().y - 0.125f * alvo.getRadius());
    alvo_background.setPosition(2 * alvo_background.getOrigin().x - 75, alvo_background.getOrigin().y);
    alvo_background.setFillColor(inverter(alvo.getFillColor()));
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

int calcular_pontuacao(sf::Color a, sf::Color b)
{
    return static_cast<int>(std::ceil(std::sqrt(2 * 255 * 255)) -
                            std::sqrt(std::pow(a.r - b.r, 2) +
                                      std::pow(a.g - b.g, 2) +
                                      std::pow(a.b - b.b, 2)));
}

void iniciar(std::default_random_engine &gerador, sf::CircleShape &atual, sf::CircleShape &alvo,
             sf::CircleShape &preview, sf::RectangleShape &alvo_background, sf::Font &fonte, sf::Text &alvo_txt,
             sf::Text &pontuacao_txt)
{
    gerador.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    criar_formas(gerador, atual, alvo, preview, alvo_background);
    fonte.loadFromFile("rsc/arial.ttf");
    alvo_txt = criar_texto("Cor-Alvo", fonte, 20, alvo.getFillColor(),
                           alvo_background.getPosition().x - alvo_background.getOrigin().x,
                           alvo_background.getPosition().y - alvo_background.getOrigin().y);
    pontuacao_txt = criar_texto("", fonte, 48);
}

void novo_jogo(std::default_random_engine &gerador, avl_t &arvore, sf::Color cor_base, apontador_t &raiz_atual,
               Botao *&bt, Botao *&bt2, unsigned altura)
{
    sf::Uint8 componente_constante;
    sf::Color (*gerador_de_cor)(std::default_random_engine &, unsigned) = nullptr;

    criaArvore(&arvore);
    componente_constante = escolher_componente_constante(gerador, cor_base, &gerador_de_cor);

    unsigned limite = static_cast<unsigned>(1 << altura);

    for (unsigned k = 0; k < limite; ++k)
        inserir(&arvore, {(*gerador_de_cor)(gerador, componente_constante)});

    raiz_atual = arvore;

    bt = new Botao(200, 300, 50.0f);
    bt2 = new Botao(600, 300, 50.0f);

    bt->set_cor(raiz_atual->esquerda->item.cor);
    bt2->set_cor(raiz_atual->direita->item.cor);
}

void desenhar_jogo(sf::RenderWindow &janela, sf::RectangleShape &alvo_background, sf::Text &alvo_txt, Botao *&bt,
                   Botao *&bt2, sf::Event::MouseMoveEvent event, sf::CircleShape &preview, sf::CircleShape &atual,
                   sf::CircleShape &alvo)
{
    janela.draw(alvo_background);
    janela.draw(alvo_txt);
    janela.draw(bt->get_circulo());
    janela.draw(bt2->get_circulo());

    if (bt->clicado(event) || bt2->clicado(event))
        janela.draw(preview);

    janela.draw(alvo);
    janela.draw(atual);
}

void desenhar_fim_de_jogo(sf::RenderWindow &janela, sf::Text pontuacao_txt, sf::CircleShape &atual,
                          sf::CircleShape &alvo)
{
    janela.draw(pontuacao_txt);
    janela.draw(alvo);
    janela.draw(atual);
}