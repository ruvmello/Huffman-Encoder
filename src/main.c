#include <stdio.h>
#include <string.h>
#include "huffman/encoder.h"
#include "huffman/decoder.h"
#include "tests/tests.h"

/*
 * De main functie, hierin wordt onderschijdt gemaakt van de argumenten
 */
int main( int argc, char *argv[] ) {

    if (strcmp(argv[1], "-c") == 0 && argc == 3) { // 3 argumenten

        char* len = strtok(argv[2], ",");
        char* mem = strtok(NULL, ",");

        if(len != NULL && strtok(NULL, ",") != NULL){
            printf("Te veel argumenten gegeven bij optie -c\n");
        } else if(mem == NULL){
            printf("Te weinig argumenten gegeven bij optie -c\n");
        } else {
            encode((unsigned int) (*len - '0'), (unsigned int) (*mem - '0'));
        }

    } else if(strcmp(argv[1], "-d") == 0 && argc == 2){ // 2 argumenten
        decode(100);
    } else if(strcmp(argv[1], "-t") == 0 && argc == 2){
        run_tests();
    } else {
        printf ("USAGE: ./persen [-d|-c]\n");
    }
    return 0;
}
