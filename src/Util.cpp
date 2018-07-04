#include "Util.h"

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

sf::Uint8 escolher_componente_constante(std::default_random_engine &gerador, sf::Color cor,
                                        sf::Color (**gerador_de_cor)(std::default_random_engine &, unsigned))
{
    std::uniform_int_distribution<int> cor_constante(0, 2);

    switch(cor_constante(gerador)) {
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