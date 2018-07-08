#ifndef AVL_H
#define AVL_H

#include <SFML/Graphics/Color.hpp>

#define DEBUG 0

typedef struct {
    sf::Color cor;
} item_t;

typedef struct structNo {
    struct structNo *esquerda;
    struct structNo *direita;
    item_t item;
    int balanceamento;
} no_t;

typedef no_t* apontador_t;

typedef apontador_t avl_t;

void criaArvore(avl_t *arvore);

bool inserir(avl_t *arvore, item_t conteudo);
void remover(avl_t *arvore, sf::Color c);

void esvaziarArvore(avl_t *arvore);

#endif //AVL_H
