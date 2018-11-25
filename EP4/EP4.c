#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct tipo_palavra{
        char* chave;
        unsigned int ocorrencias;
}tipo_palavra;

typedef struct elo_ll{
    struct elo_ll* prox;
    tipo_palavra* palavra;
}elo_ll;

elo_ll* criaElo(char* palavra, unsigned int ocorrencias){
    elo_ll* elo = malloc(sizeof(elo_ll));
    elo->prox = NULL;
    int tam = strlen(palavra);
    elo->palavra = malloc(sizeof(tipo_palavra));
    elo->palavra->chave = malloc((tam + 1) * sizeof(char));
    strcpy(elo->palavra->chave, palavra);
    elo->palavra->ocorrencias = ocorrencias;
    elo->prox = NULL;
    return elo;
}

elo_ll* buscaDesordenadaLl(elo_ll* lista, char* palavra){
    elo_ll* elo = lista;
    while(elo){
        if(strcmp(elo->palavra->chave, palavra) == 0) return elo;
        elo = elo->prox;
    }
    return NULL;
}

elo_ll* insereNaPosLl(elo_ll* elo_pos, char* palavra, int ocorrencias){
    elo_ll* elo = criaElo(palavra, ocorrencias);
    if(elo_pos){
        elo_ll* prox = elo_pos->prox;
        elo_pos->prox = elo;
        elo->prox = prox;
    }
    return elo;
}

elo_ll* insereDesordenadoLl(elo_ll* lista, char* palavra, elo_ll* ultimo){
    if(!lista) return criaElo(palavra, 1);
    elo_ll* elo = buscaDesordenadaLl(lista, palavra);
    if(elo){
        elo->palavra->ocorrencias++;
        return elo;
    }
    return insereNaPosLl(ultimo, palavra, 1);
}

elo_ll* buscaOrdenadaLl(elo_ll* lista, char* palavra, elo_ll** pos_insere){
    elo_ll* elo = lista;
    while(elo){
        int res = strcmp(elo->palavra->chave, palavra);
        if(res == 0) return elo;
        if(res > 0){
            //printf("A inserir: \'%s\'\nComparando: \'%s\'\n", palavra, elo->palavra->chave);
            //printf("Inserir \'%s\' depois de \'%s\'\n", palavra, (*pos_insere)->palavra->chave);
            return NULL;
        } 
        *pos_insere = elo;
        elo = elo->prox;
    }
    return NULL;
}

elo_ll* insereOrdenadoLl(elo_ll* lista, char* palavra){
    elo_ll* pos = lista;
    elo_ll* elo = buscaOrdenadaLl(lista, palavra, &pos);
    if(elo){
        elo->palavra->ocorrencias++;
        return elo;
    }
    elo = criaElo(palavra, 1);
    if(pos){
        elo_ll* prox = pos->prox;
        pos->prox = elo;
        elo->prox = prox;
    }
    return elo;
}

void percorreLl(elo_ll* lista){
    if(!lista) printf("LISTA VAZIA!\n");
    while(lista){
        printf("%s %d\n", lista->palavra->chave, lista->palavra->ocorrencias);
        lista = lista->prox;
    }
}



int main(int argc, char const *argv[]){
    if(argc < 2){
        fprintf(stderr, "Número incorreto de parâmetros!\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen(argv[1], "r");
    
    if(!fp){
        perror("Error");
        exit(EXIT_FAILURE);
    }

    elo_ll* listaDes = NULL;
    elo_ll* ultimo = listaDes;

    char in[255]; 
    while(fgets(in, 255, fp)){
        char* c = in;
        char* word = strtok(c, "\" \t ( ) { } [ ] . ; , \n \' - ? ! :");
        while(word){
            for(int i = 0; word[i] != '\0'; i++){
                word[i] = tolower(word[i]);
            }
            ultimo = insereOrdenadoLl(listaDes, word);
            if(!listaDes) listaDes = ultimo;
            word = strtok(NULL, "\" \t ( ) { } [ ] . ; , \n \' - ? ! :");
        }
    }
    percorreLl(listaDes);
    return 0;
}

