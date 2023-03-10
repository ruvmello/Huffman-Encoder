//
// Created by ruben on 24-10-2021.
//

#include "decoder.h"
#include <stdio.h>

char input = '\0';
unsigned int index_in_char = 0;

/*
 * Hoofdfunctie die alles decodeert
 */
void decode(int n){
    char buffer[n];
    unsigned int a = fread(buffer, sizeof(char), n, stdin);

    read_byte(buffer[0]);

    unsigned int i = 1; // Volg het aantal verwerkte characters uit de buffer

    Node* tree = krijg_nng();
    while(a != 0){
        Node* curr = tree;
        // Volg het pad uit de encoding
        while(curr->ordenummer != 1 && curr->label == NULL){

            if(read_bit_at_current_index()){
                curr = curr->right_child;
            } else {
                curr = curr->left_child;
            }

            index_in_char = (index_in_char + 1) % 8;
            if(index_in_char == 0){
                read_byte(buffer[i]);
                i++;

                if(i == a){
                    a = fread(buffer, sizeof(char), n, stdin);
                    i = 0;
                }
            }
        }

        // Als het label niet NULL is, dan zat het al in de boom
        if(curr->label != NULL){
            pas_boom_aan(curr->label, tree, curr, 1); // Len doet er hier niet toe
            fwrite(curr->label , curr->size, sizeof(char) , stdout);
        } else {
            // lees hoeveel characters er volgen
            unsigned int aantal_chars = 0;
            for (unsigned int j = 0; j <= 7; j++) {
                aantal_chars |= (read_bit_at_current_index()) << (7 - j);
                index_in_char = (index_in_char + 1) % 8;
                if (index_in_char == 0) {
                    read_byte(buffer[i]);
                    i++;

                    if (i == a) {
                        a = fread(buffer, sizeof(char), n, stdin);
                        i = 0;
                    }
                }
            }

            if(aantal_chars == 0){
                break;
            }

            char chars[aantal_chars];
            for(unsigned int k = 0; k < aantal_chars; k++) { // Voor het aantal karakters dat worden toegevoegd
                // Output het nieuwe character dat wordt toegevoegd aan de boom
                char c = 0; // c = 0000 0000
                for (unsigned int j = 0; j <= 7; j++) {
                    c |= (read_bit_at_current_index()) << (7 - j);
                    index_in_char = (index_in_char + 1) % 8;

                    // Indien een char is verwerkt, laad de nieuwe in
                    if (index_in_char == 0) {
                        read_byte(buffer[i]);
                        i++;

                        if (i == a) {
                            a = fread(buffer, sizeof(char), n, stdin);
                            i = 0;
                        }
                    }
                }
                chars[k] = c;
            }
            pas_boom_aan(chars, tree, curr, aantal_chars);
            fwrite(chars , aantal_chars, sizeof(char) , stdout);
        }
    }
    // Free de boom
    clean_up_tree(tree);
}

/*
 * Lees een character in naar een globale variabele input
 */
void read_byte(char ch){
    input = ch;
}

/*
 * Geeft de bit terug op de huidige index
 */
unsigned int read_bit_at_current_index(){
    return (input >> (7 - index_in_char) & '\1'); // Schuif de bit volledig naar rechts en doe een AND met 1
}