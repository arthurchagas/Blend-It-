#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "AVL.h"

static bool __inserir(avl_t *arvore, item_t conteudo, signed char *subArvoreCresceu);

static void __remover(avl_t *arvore, sf::Color c, signed char *subArvoreEncolheu);

static void __removerMenor(avl_t *arvore, avl_t *atual, signed char *subArvoreEncolheu);

static void __removerMaior(avl_t *arvore, avl_t *atual, signed char *subArvoreEncolheu);

static void __balancearEsquerda(avl_t *arvore, signed char *subArvoreEncolheu);

static void __balancearDireita(avl_t *arvore, signed char *subArvoreEncolheu);

static void __rotacaoDireitaSimples(avl_t *arvore, signed char *subArvoreEncolheu);

static void __rotacaoEsquerdaSimples(avl_t *arvore, signed char *subArvoreEncolheu);

static void __rotacaoDireitaDupla(avl_t *arvore, char remocao);

static void __rotacaoEsquerdaDupla(avl_t *arvore, char remocao);

static void __criaRaiz(avl_t *arvore, item_t conteudo);

void criaArvore(avl_t *arvore)
{
    *arvore = NULL;
}

bool inserir(avl_t *arvore, item_t conteudo)
{
    signed char subArvoreCresceu = 0;

    if (DEBUG)
        printf("Inserindo (%d, %d, %d)...", conteudo.cor.r, conteudo.cor.g, conteudo.cor.b);
    return __inserir(arvore, conteudo, &subArvoreCresceu);
}

void remover(avl_t *arvore, sf::Color c)
{
    signed char subArvoreEncolheu = 0;

    if (DEBUG)
        printf("Removendo (%d, %d, %d)...", c.r, c.g, c.b);
    __remover(arvore, c, &subArvoreEncolheu);
}

void esvaziarArvore(avl_t *arvore)
{
    if (*arvore == NULL)
        return;

    esvaziarArvore(&(*arvore)->esquerda);
    esvaziarArvore(&(*arvore)->direita);
    remover(arvore, (*arvore)->item.cor);
}

static void __criaRaiz(avl_t *arvore, item_t conteudo)
{
    *arvore = (avl_t)malloc(sizeof(no_t));

    (*arvore)->balanceamento = 0;
    (*arvore)->item = conteudo;
    (*arvore)->direita = NULL;
    (*arvore)->esquerda = NULL;
}

static bool __inserir(avl_t *arvore, item_t conteudo, signed char *subArvoreCresceu)
{
    if (*arvore == NULL)
    {
        __criaRaiz(arvore, conteudo);
        *subArvoreCresceu = 1;
        if (DEBUG)
            printf("Sucesso!\n");
        return true;
    }

    signed char subArvoreEncolheu = -1;
    bool sucesso;

    if ((*arvore)->item.cor.toInteger() > conteudo.cor.toInteger())
    {
        sucesso = __inserir(&(*arvore)->esquerda, conteudo, subArvoreCresceu);

        if (*subArvoreCresceu)
        {
            if ((*arvore)->balanceamento == 1)
            {
                if ((*arvore)->esquerda->balanceamento == 1)
                    __rotacaoDireitaSimples(arvore, &subArvoreEncolheu);
                else
                    __rotacaoDireitaDupla(arvore, 0);
                (*arvore)->balanceamento = 0;
                *subArvoreCresceu = 0;
            }
            else if ((*arvore)->balanceamento == -1)
            {
                ++(*arvore)->balanceamento;
                *subArvoreCresceu = 0;
            }
            else
                ++(*arvore)->balanceamento;
        }
    }
    else if ((*arvore)->item.cor.toInteger() < conteudo.cor.toInteger())
    {
        sucesso = __inserir(&(*arvore)->direita, conteudo, subArvoreCresceu);

        if (*subArvoreCresceu)
        {
            if ((*arvore)->balanceamento == -1)
            {
                if ((*arvore)->direita->balanceamento == -1)
                    __rotacaoEsquerdaSimples(arvore, &subArvoreEncolheu);
                else
                    __rotacaoEsquerdaDupla(arvore, 0);
                (*arvore)->balanceamento = 0;
                *subArvoreCresceu = 0;
            }
            else if ((*arvore)->balanceamento == 1)
            {
                --(*arvore)->balanceamento;
                *subArvoreCresceu = 0;
            }
            else
                --(*arvore)->balanceamento;
        }
    }
    else
    {
        if (DEBUG)
            printf("Erro! Chave j existe\n");
        sucesso = false;
    }

    return sucesso;
}

static void __remover(avl_t *arvore, sf::Color c, signed char *subArvoreEncolheu)
{
    if (*arvore == NULL)
    {
        if (DEBUG)
            printf("Erro: chave nao encontrada!\n");
        return;
    }

    if ((*arvore)->item.cor.toInteger() > c.toInteger())
    {
        __remover(&(*arvore)->esquerda, c, subArvoreEncolheu);

        if (*subArvoreEncolheu)
            __balancearEsquerda(arvore, subArvoreEncolheu);

        return;
    }
    else if ((*arvore)->item.cor.toInteger() < c.toInteger())
    {
        __remover(&(*arvore)->direita, c, subArvoreEncolheu);

        if (*subArvoreEncolheu)
            __balancearDireita(arvore, subArvoreEncolheu);

        return;
    }

    apontador_t aux = *arvore;

    if (aux->direita == NULL)
    {
        (*arvore) = aux->esquerda;
        *subArvoreEncolheu = 1;
    }
    else if (aux->esquerda == NULL)
    {
        (*arvore) = aux->direita;
        *subArvoreEncolheu = 1;
    }
    else
    {
        __removerMenor(&aux, &aux->direita, subArvoreEncolheu);

        if (*subArvoreEncolheu)
            __balancearDireita(arvore, subArvoreEncolheu);
    }

    free(aux);
    if (DEBUG)
        printf("Sucesso!\n");
}

static void __removerMenor(avl_t *arvore, avl_t *atual, signed char *subArvoreEncolheu)
{
    if ((*atual)->esquerda != NULL)
    {
        __removerMenor(arvore, &(*atual)->esquerda, subArvoreEncolheu);

        if (*subArvoreEncolheu)
            __balancearEsquerda(atual, subArvoreEncolheu);

        return;
    }

    (*arvore)->item = (*atual)->item;
    *arvore = *atual;
    (*atual) = (*atual)->direita;
    *subArvoreEncolheu = 1;
}

static void __removerMaior(avl_t *arvore, avl_t *atual, signed char *subArvoreEncolheu)
{
    if ((*atual)->direita != NULL)
    {
        __removerMaior(arvore, &(*atual)->direita, subArvoreEncolheu);

        if (*subArvoreEncolheu)
            __balancearDireita(atual, subArvoreEncolheu);

        return;
    }

    (*arvore)->item = (*atual)->item;
    *arvore = *atual;
    (*atual) = (*atual)->esquerda;
    *subArvoreEncolheu = 1;
}

static void __balancearEsquerda(avl_t *arvore, signed char *subArvoreEncolheu)
{
    if ((*arvore)->balanceamento == 1)
    {
        (*arvore)->balanceamento = 0;

        return;
    }
    else if ((*arvore)->balanceamento == 0)
    {
        (*arvore)->balanceamento = -1;
        *subArvoreEncolheu = 0;

        return;
    }

    if ((*arvore)->direita->balanceamento <= 0)
    {
        __rotacaoEsquerdaSimples(arvore, subArvoreEncolheu);

        return;
    }

    __rotacaoEsquerdaDupla(arvore, 1);
}

static void __balancearDireita(avl_t *arvore, signed char *subArvoreEncolheu)
{
    if ((*arvore)->balanceamento == -1)
    {
        (*arvore)->balanceamento = 0;
        return;
    }
    else if ((*arvore)->balanceamento == 0)
    {
        (*arvore)->balanceamento = +1;
        *subArvoreEncolheu = 0;
        return;
    }

    if ((*arvore)->esquerda->balanceamento >= 0)
    {
        __rotacaoDireitaSimples(arvore, subArvoreEncolheu);

        return;
    }

    __rotacaoDireitaDupla(arvore, 1);
}

static void __rotacaoDireitaSimples(avl_t *arvore, signed char *subArvoreEncolheu)
{
    if (DEBUG)
        printf("Rotacao simples 'a direita!\n");

    apontador_t aux = (*arvore)->esquerda;
    int auxBalanceamento = aux->balanceamento;

    (*arvore)->esquerda = aux->direita;
    aux->direita = *arvore;

    if (*subArvoreEncolheu != -1)
        if (auxBalanceamento == 0)
        {
            (*arvore)->balanceamento = 1;
            aux->balanceamento = -1;
            *subArvoreEncolheu = 0;
        }
        else
        {
            (*arvore)->balanceamento = 0;
            aux->balanceamento = 0;
        }
    else
        (*arvore)->balanceamento = 0;

    *arvore = aux;
}

static void __rotacaoEsquerdaSimples(avl_t *arvore, signed char *subArvoreEncolheu)
{
    if (DEBUG)
        printf("Rotacao simples 'a esquerda!\n");

    apontador_t aux = (*arvore)->direita;
    int auxBalanceamento = aux->balanceamento;

    (*arvore)->direita = aux->esquerda;
    aux->esquerda = *arvore;

    if (*subArvoreEncolheu != -1)
    {
        if (auxBalanceamento == 0)
        {
            (*arvore)->balanceamento = -1;
            aux->balanceamento = 1;
            *subArvoreEncolheu = 0;
        }
        else
        {
            (*arvore)->balanceamento = 0;
            aux->balanceamento = 0;
        }
    }
    else
        (*arvore)->balanceamento = 0;

    *arvore = aux;
}

static void __rotacaoDireitaDupla(avl_t *arvore, char remocao)
{
    if (DEBUG)
        printf("Rotacao dupla 'a direita!\n");

    apontador_t aux = (*arvore)->esquerda;
    apontador_t aux2 = aux->direita;
    int auxBalanceamento2 = aux2->balanceamento;

    aux->direita = aux2->esquerda;
    aux2->esquerda = aux;
    (*arvore)->esquerda = aux2->direita;
    aux2->direita = *arvore;

    if (remocao)
    {
        if (auxBalanceamento2 == 0 || auxBalanceamento2 == -1)
        {
            (*arvore)->balanceamento = 0;

            if (auxBalanceamento2 == -1)
                aux->balanceamento = 1;
        }

        if (auxBalanceamento2 == 0 || auxBalanceamento2 == 1)
        {
            aux->balanceamento = 0;

            if (auxBalanceamento2 == 1)
                (*arvore)->balanceamento = -1;
        }
    }
    else
    {
        if (aux2->balanceamento == 0 || aux2->balanceamento == -1)
        {
            (*arvore)->balanceamento = 0;

            if (aux2->balanceamento == -1)
                aux->balanceamento = 1;
        }

        if (aux2->balanceamento == 0 || aux2->balanceamento == 1)
        {
            aux->balanceamento = 0;

            if (aux2->balanceamento == 1)
                (*arvore)->balanceamento = -1;
        }
    }

    *arvore = aux2;

    if (remocao)
        aux2->balanceamento = 0;
}

static void __rotacaoEsquerdaDupla(avl_t *arvore, char remocao)
{
    if (DEBUG)
        printf("Rotacao dupla 'a esquerda!\n");

    apontador_t aux = (*arvore)->direita;
    apontador_t aux2 = aux->esquerda;
    int aux2Balanceamento = aux2->balanceamento;

    aux->esquerda = aux2->direita;
    aux2->direita = aux;
    (*arvore)->direita = aux2->esquerda;
    aux2->esquerda = *arvore;

    if (remocao)
    {
        if (aux2Balanceamento == 0 || aux2Balanceamento == -1)
        {
            aux->balanceamento = 0;

            if (aux2Balanceamento == -1)
                (*arvore)->balanceamento = 1;
        }

        if (aux2Balanceamento == 0 || aux2Balanceamento == 1)
        {
            (*arvore)->balanceamento = 0;

            if (aux2Balanceamento == 1)
                aux->balanceamento = -1;
        }
    }
    else
    {
        if (aux2->balanceamento == 0 || aux2->balanceamento == -1)
        {
            aux->balanceamento = 0;

            if (aux2->balanceamento == -1)
                (*arvore)->balanceamento = 1;
        }

        if (aux2->balanceamento == 0 || aux2->balanceamento == 1)
        {
            (*arvore)->balanceamento = 0;

            if (aux2->balanceamento == 1)
                aux->balanceamento = -1;
        }
    }

    *arvore = aux2;

    if (remocao)
        aux2->balanceamento = 0;
}