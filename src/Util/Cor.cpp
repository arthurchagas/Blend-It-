#include "Util/Cor.h"

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

double distancia(sf::Color a, sf::Color b)
{
    return std::sqrt(std::pow(a.r - b.r, 2) + std::pow(a.g - b.g, 2) + std::pow(a.b - b.b, 2));
}