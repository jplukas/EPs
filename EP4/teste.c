#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){
    if(argc < 2){
        fprintf(stderr, "Numero incorreto de parametros!\n");
        exit(EXIT_FAILURE);
    }
    FILE* fp = fopen(argv[1], "r");
    if(!fp){
        perror("Error");
        exit(EXIT_FAILURE);
    }
    char* c = strtok(argv[1], "\"");
    while(c){
        printf("|%s|\n", c);
        c = strtok(NULL, "\"");
    }
    return 0;
}
