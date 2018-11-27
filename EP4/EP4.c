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

typedef struct arvore{
    tipo_palavra* palavra;
    struct arvore* esq;
    struct arvore* dir;
}arvore;

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
    int res = n2 - n1;
    if(res == 0) res = strcmp((*palavra1)->chave, (*palavra2)->chave);
    return res;
}

void percorreVetor(tipo_palavra** palavras, int tamanho_vetor){
    for(int i = 0; i < tamanho_vetor; i++){
        printf("%s %d\n", palavras[i]->chave, palavras[i]->ocorrencias);
    }
}

int carregaListaVetor(elo_ll* lista, tipo_palavra** palavras){
    int tamanho_vetor = 0;
    elo_ll* elo = lista;
    while(elo){
        if(tamanho_vetor >= NUM_PALAVRAS){
            printf("Sem memória!\n");
            exit(EXIT_FAILURE);
        }
        palavras[tamanho_vetor++] = elo->palavra;
        elo_ll* ant = elo;
        elo = elo->prox;
        ant->prox = NULL;
        free(ant);
    }
    return tamanho_vetor;
}

int buscaBinariaVetor(tipo_palavra** palavras, int tamanho_vetor, char* palavra, int* pos_insere){
    int minimo = 0;
    int maximo = tamanho_vetor - 1;
    while(maximo >= minimo){
        int alvo = (maximo + minimo)/2;
        *pos_insere = alvo;
        int comp = strcmp(palavras[alvo]->chave, palavra);
        if(comp == 0) return alvo;
        if(comp > 0)maximo = alvo - 1;
        else{
            minimo = alvo + 1;
            (*pos_insere)++;
        }
    }
    return -1;
}


int insereVetorOrd(tipo_palavra** palavras, int tamanho_vetor, char* palavra){
    int pos_insere = 0;
    int pos = buscaBinariaVetor(palavras, tamanho_vetor, palavra, &pos_insere);
    if(pos >= 0) palavras[pos]->ocorrencias++;
    else{
        for(int i = tamanho_vetor; i > pos_insere; i--)
            palavras[i] = palavras[i - 1];
        adicionaVetorNaPos(palavras, pos_insere, palavra);
        tamanho_vetor++;
    }
    return tamanho_vetor;
}

arvore* buscaBinariaArvore(arvore* a, char* palavra, arvore** ult){
    while(a){
        *ult = a;
        int comp = strcmp(a->palavra->chave, palavra);
        if(comp == 0) return a;
        if(comp > 0) a = a->esq;
        else a = a->dir;
    }
    return NULL;
}

arvore* insereNaArvore(arvore* a, char* palavra){
        if(!a){
            a = malloc(sizeof(arvore));
            a->esq = NULL;
            a->dir = NULL;
            a->palavra = malloc(sizeof(tipo_palavra));
            int tam = strlen(palavra) + 1;
            a->palavra->chave = malloc(tam * sizeof(char));
            strcpy(a->palavra->chave, palavra);
            a->palavra->ocorrencias = 1;
            return a;
        }
        arvore* ult = NULL;

        arvore* no = buscaBinariaArvore(a, palavra, &ult);
        if(no) no->palavra->ocorrencias++;
        else{
            no = malloc(sizeof(arvore));
            no->esq = NULL;
            no->dir = NULL;
            no->palavra = malloc(sizeof(tipo_palavra));
            int tam = strlen(palavra) + 1;
            no->palavra->chave = malloc(tam * sizeof(char));
            strcpy(no->palavra->chave, palavra);
            no->palavra->ocorrencias = 1;
            if(strcmp(palavra, ult->palavra->chave) > 0) ult->dir = no;
            else ult->esq = no;
        }
        return a;
}

void percorreArvore(arvore* a){
    if(!a) return;
    percorreArvore(a->esq);
    printf("%s %d\n", a->palavra->chave, a->palavra->ocorrencias);
    percorreArvore(a->dir);
}

void carregaArvoreVetor(arvore* a, tipo_palavra** palavras, int* pos_vetor){
    if(!a) return;
    if(*pos_vetor >= NUM_PALAVRAS){
        printf("Sem memória!\n");
        exit(EXIT_FAILURE);
    }
    carregaArvoreVetor(a->esq, palavras, pos_vetor);
    palavras[(*pos_vetor)++] = a->palavra;
    carregaArvoreVetor(a->dir, palavras, pos_vetor);
}

void decideEstrutura(tipo_palavra** palavras, int* tamanho_vetor, elo_ll** lista, elo_ll** ultimo, \
arvore** tree, char* estrutura, char* palavra, int* decisao){
    if(strcmp(estrutura, "VD") == 0){
        *tamanho_vetor = insereVetorDes(palavras, *tamanho_vetor, palavra);
        *decisao = 1;
    }
    else{
        if(strcmp(estrutura, "VO") == 0){
            *tamanho_vetor = insereVetorOrd(palavras, *tamanho_vetor, palavra);
            *decisao = 2;
        }
        else{
            if(strcmp(estrutura, "LD") == 0){
                *lista = insereDesordenadoLl(*lista, palavra, ultimo);
                *decisao = 3;
            }
            else{
                if(strcmp(estrutura, "LO") == 0){
                    *lista = insereOrdenadoLl(*lista, palavra);
                    *decisao = 4;
                }
                else{
                    if(strcmp(estrutura, "AB") == 0){
                        *tree = insereNaArvore(*tree, palavra);
                        *decisao = 5;
                    }
                    else{
                        printf("OPCAO INVALIDA!\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }
}

void mostraResultados(tipo_palavra** palavras, int tamanho_vetor, elo_ll* lista, \
arvore* tree, int decisao, char* ordenacao){
    int (*compare)(const void*, const void*) = NULL;
    int o = -1;
    if(strcmp(ordenacao, "A") == 0){
       compare = comparaPalavrasChave;
       o = 0;
    }
    else{
        if(strcmp(ordenacao, "O") == 0){
            compare = comparaPalavrasOcorr;
            o = 1;
        }
        else{
            printf("OPCAO INVALIDA!\n");
            exit(EXIT_FAILURE);
        }
    }
    int zero = 0;
    switch(decisao){
        case 1:
            qsort(palavras, tamanho_vetor, sizeof(tipo_palavra*), compare);
            break;
        
        case 2:
            if(o) qsort(palavras, tamanho_vetor, sizeof(tipo_palavra*), compare);
            break;

        case 3:
            tamanho_vetor = carregaListaVetor(lista, palavras);
            qsort(palavras, tamanho_vetor, sizeof(tipo_palavra*), compare);
            break;
        
        case 4:
            tamanho_vetor = carregaListaVetor(lista, palavras);
            if(o) qsort(palavras, tamanho_vetor, sizeof(tipo_palavra*), compare);
            break;

        case 5:
            carregaArvoreVetor(tree, palavras, &zero);
            tamanho_vetor = zero;
            if(o) qsort(palavras, tamanho_vetor, sizeof(tipo_palavra*), compare);
            break;
        
        default:
            printf("OPCAO INVALIDA!\n");
            exit(EXIT_FAILURE);
            break;
    }
    percorreVetor(palavras, tamanho_vetor);
}

int main(int argc, char const *argv[]){
    if(argc < 4){
        fprintf(stderr, "Número incorreto de parâmetros!\n");
        exit(EXIT_FAILURE);
    }

    char* estrutura = argv[2];
    char* ordenacao = argv[3];

    FILE* fp = fopen(argv[1], "r");

    if(!fp){
        perror("Error");
        exit(EXIT_FAILURE);
    }
    tipo_palavra* palavras[NUM_PALAVRAS];
    int tamanho_vetor = 0; 
    elo_ll* lista = NULL;
    elo_ll* ultimo = NULL;
    arvore* tree = NULL;
    char in[255];
    int decisao = -1;

    while(fgets(in, 255, fp)){
        const char* delim = "\" \t ( ) { } [ ] . ; , \n \' - ? ! : \\ / $ # * @";
        char* word = strtok(in, delim);
        while(word){
            for(int i = 0; word[i] != '\0'; i++){
                word[i] = tolower(word[i]);
            }
            decideEstrutura(palavras, &tamanho_vetor, &lista, &ultimo, &tree, estrutura, word, &decisao);
            word = strtok(NULL, delim);
        }
    }
    mostraResultados(palavras, tamanho_vetor, lista, tree, decisao, ordenacao);
    return 0;
}

