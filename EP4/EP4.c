#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_PALAVRAS 10000

typedef struct tipo_palavra{
        char* chave;
        int ocorrencias;
}tipo_palavra;

typedef struct elo_ll{
    struct elo_ll* prox;
    tipo_palavra* palavra;
}elo_ll;

elo_ll* criaElo(char* palavra, int ocorrencias){
    elo_ll* elo = malloc(sizeof(elo_ll));
    int tam = strlen(palavra);
    elo->palavra = malloc(sizeof(tipo_palavra));
    elo->palavra->chave = malloc((tam + 1) * sizeof(char));
    strcpy(elo->palavra->chave, palavra);
    elo->palavra->ocorrencias = ocorrencias;
    elo->prox = NULL;
    return elo;
}

elo_ll* buscaDesordenadaLl(elo_ll* lista, char* palavra, elo_ll** ultimo){
    elo_ll* elo = lista;
    while(elo){
        if(strcmp(elo->palavra->chave, palavra) == 0){
            //printf("Palavra encontrada: %s. %x\n", elo->palavra->chave, (elo->palavra->ocorrencias + 1));
            return elo;
        }
        *ultimo = elo;
        elo = elo->prox;
    }
    //if(*ultimo) printf("Último item da lista: %s\n", (*ultimo)->palavra->chave);
    return NULL;
}

elo_ll* insereNaPosLl(elo_ll* elo_pos, char* palavra, int ocorrencias){
    elo_ll* elo = criaElo(palavra, ocorrencias);
    if(elo_pos){
        elo_ll* prox = elo_pos->prox;
        elo_pos->prox = elo;
        elo->prox = prox;
    }
    else{
        elo_pos = elo;
    }
    return elo_pos;
}

elo_ll* insereDesordenadoLl(elo_ll* lista, char* palavra, elo_ll** ultimo){
    elo_ll* elo = buscaDesordenadaLl(lista, palavra, ultimo);
    if(elo){
        elo->palavra->ocorrencias++;
    }
    else{
        *ultimo = insereNaPosLl(*ultimo, palavra, 1);
        if(!lista) lista = *ultimo;
    }
    return lista;
}

elo_ll* buscaOrdenadaLl(elo_ll* lista, char* palavra, elo_ll** pos_insere){
    elo_ll* elo = lista;
    while(elo){
        int res = strcmp(elo->palavra->chave, palavra);
        if(res == 0) return elo;
        if(res > 0){
            return NULL;
        } 
        *pos_insere = elo;
        elo = elo->prox;
    }
    return NULL;
}

//retorna primeiro elemento da lista
elo_ll* insereOrdenadoLl(elo_ll* lista, char* palavra){
    elo_ll* pos = NULL;
    if(!lista){
        lista = criaElo(palavra, 1);
        return lista;
    }
    elo_ll* elo = buscaOrdenadaLl(lista, palavra, &pos);
    if(elo){
        elo->palavra->ocorrencias++;
        return lista;
    }
    elo = criaElo(palavra, 1);
    if(pos){
        elo_ll* prox = pos->prox;
        pos->prox = elo;
        elo->prox = prox;
    }
    else{
        elo_ll* prox = lista->prox;
        lista = elo;
        elo->prox = prox;
    }
    return lista;
}

void percorreLl(elo_ll* lista){
    if(!lista) printf("LISTA VAZIA!\n");
    while(lista){
        printf("%s %d\n", lista->palavra->chave, lista->palavra->ocorrencias);
        lista = lista->prox;
    }
}

void adicionaVetorNaPos(tipo_palavra** palavras, int pos, char* palavra){
    if(pos >= NUM_PALAVRAS) return;
    tipo_palavra* p = malloc(sizeof(tipo_palavra));
    p->chave = malloc((strlen(palavra) + 1) * sizeof(char));
    strcpy(p->chave, palavra);
    p->ocorrencias = 1;
    palavras[pos] = p;
}

tipo_palavra* buscaVetorDes(tipo_palavra** palavras, int tamanho_vetor, char* palavra){
    tipo_palavra* p = NULL;
    for(int i = 0; i < tamanho_vetor; i++){
        if(strcmp(palavras[i]->chave, palavra) == 0){
            p = palavras[i];
            break;
        }
    }
    return p;
}

int insereVetorDes(tipo_palavra** palavras, int tamanho_vetor, char* palavra){
    if(tamanho_vetor >= NUM_PALAVRAS) return -1;
    tipo_palavra* p = buscaVetorDes(palavras, tamanho_vetor, palavra);
    if(p) p->ocorrencias++;
    else adicionaVetorNaPos(palavras, tamanho_vetor++, palavra);
    return tamanho_vetor;
}

int comparaPalavrasChave(const void* a, const void* b){
    tipo_palavra** palavra1 = (tipo_palavra**)a;
    tipo_palavra** palavra2 = (tipo_palavra**)b;
    
    char* str1 = (*palavra1)->chave;
    char* str2 = (*palavra2)->chave;
    return strcmp(str1, str2);
}

int comparaPalavrasOcorr(const void* a, const void* b){
    tipo_palavra** palavra1 = (tipo_palavra**)a;
    tipo_palavra** palavra2 = (tipo_palavra**)b;

    int n1 = (*palavra1)->ocorrencias;
    int n2 = (*palavra2)->ocorrencias;
    int res = n1 - n2;
    if(res == 0) res = strcmp((*palavra1)->chave, (*palavra2)->chave);
    return res;
}

void percorreVetor(tipo_palavra** palavras, int tamanho_vetor){
    for(int i = 0; i < tamanho_vetor; i++){
        printf("%s %d\n", palavras[i]->chave, palavras[i]->ocorrencias);
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

    tipo_palavra* palavras[NUM_PALAVRAS];
    int tamanho_vetor = 0; 

    char in[255];
    while(fgets(in, 255, fp)){
        char* c = in;
        const char* delim = "\" \t ( ) { } [ ] . ; , \n \' - ? ! : \\ / $ # * @";
        char* word = strtok(c, delim);
        while(word){
            for(int i = 0; word[i] != '\0'; i++){
                word[i] = tolower(word[i]);
            }
            tamanho_vetor = insereVetorDes(palavras, tamanho_vetor, word);
            if(tamanho_vetor >= NUM_PALAVRAS - 1){
                printf("\n\n\n\n\n\n\nERRO\n\n\n\n\n\n\n\n\n");
                printf("FALTA DE MEMORIA!\n\n\n\n");
            }
            word = strtok(NULL, delim);
        }
    }
    qsort(palavras, tamanho_vetor, sizeof(tipo_palavra*), comparaPalavrasChave);
    percorreVetor(palavras, tamanho_vetor);
    return 0;
}

