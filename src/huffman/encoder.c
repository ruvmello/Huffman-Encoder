//
// Created by ruben on 17-10-2021.
//

#include "encoder.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

unsigned int index_inside_char = 0;
char char_for_stdout = '\0';

/*
 * De functie die alles encodeert door de juiste functies op te roepen,
 * deze funtie wordt opgeroepen uit main
 */
void encode(unsigned int max_len, unsigned int lim){
    unsigned int buffer_size = max_len * 50;
    char buffer[buffer_size];
    unsigned int a = fread(buffer, sizeof(char), buffer_size, stdin);
    unsigned int i = 0;
    unsigned int mem_lim = get_memory_limit(lim);
    char number_of_len_1 = 0; // Aantal characters die toegevoegd zijn van lente 1, max 256

    Node* tree = krijg_nng();
    Node* z;
    while(a != 0){

        unsigned int char_len;
//        char_len = get_max_len(max_len, mem_lim, number_of_len_1);
        char_len = get_max_len(buffer, i, a, max_len, mem_lim, number_of_len_1);

        if(char_len + i > a)
            char_len = a - i;

        char sbuf[char_len];
        memcpy(sbuf, &buffer[i], char_len);

        z = vind_node(tree, sbuf, char_len, false);

        if(z->ordenummer != 1){ // z in boom
            output_of_encoding(z, tree, sbuf, char_len, true);
        } else {
            if(char_len == 1)
                number_of_len_1++;
            output_of_encoding(z, tree, sbuf, char_len, false);
        }

        pas_boom_aan(sbuf, tree, z, char_len);
        i += char_len;
        if(i == a){
            a = fread(buffer, sizeof(char), max_len, stdin);
            i = 0;
        }
    }

    // pad naar nng
    z = vind_node(tree, "", 1, true); // Label doet er hier niet toe, size ook niet
    output_of_pad(z, tree);

    // gevolgd door '00000000'
    for (unsigned int k = 0; k < 8; k++) {
        char_for_stdout = (char_for_stdout << 1);
        index_inside_char++;
        if (index_inside_char == 8) {
            output_bits_as_char();
            index_inside_char = 0;
        }
    }

    // laatste byte opvullen
    while(index_inside_char != 8){
        char_for_stdout = (char_for_stdout << 1);
        index_inside_char++;
    }

    output_bits_as_char();

    // Free de boom
    clean_up_tree(tree);
}

/*
 * Deze functie zorgt ervoor dat de juiste volgorde wordt uitgeschreven naar stdout
 * en maakt onderscheid voor labels die al in de boom zitten of niet
 */
void output_of_encoding(Node* node, Node* root, char* label, unsigned int size, bool zit_in_boom){
    output_of_pad(node, root);

    // Indien niet in boom, zeg hoeveel karakters gevolgd door char
    if(!zit_in_boom) {
        for (unsigned int i = 0; i < 8; i++) {
            char_for_stdout = (char_for_stdout << 1) +  (((size << i) & 0x80) != 0);
            index_inside_char++;
            if (index_inside_char == 8) {
                output_bits_as_char();
                index_inside_char = 0;
            }
        }

        // add label as bits to output
        for(unsigned int j = 0; j < size; j++) {
            char ch = label[j];
            for (unsigned int i = 0; i <= 7; i++) {
                char_for_stdout = (char_for_stdout << 1) + (((ch << i) & 0x80) != 0);
                index_inside_char++;
                if (index_inside_char == 8) {
                    output_bits_as_char();
                    index_inside_char = 0;
                }
            }
        }
    }
}

/*
 * Zorgt ervoor dat het pad naar nng of de node met hetzelfde label
 * wordt uitgeschreven naar stdout
 */
void output_of_pad(Node* node, Node* root){
    Node* curr;
    curr = node;
    char pad[root->ordenummer];
    int iter = 0;
    // pad naar node
    while(curr->parent != NULL) {
        if (curr->parent->left_child == curr) {
            pad[iter] = '0';
        } else {
            pad[iter] = '1';
        }
        iter++;
        curr = curr->parent;
    }

    // pad omzetten naar output
    for(int k = iter - 1; k >= 0; k--){
        char_for_stdout = (char_for_stdout << 1) + pad[k] - '0';
        index_inside_char++;

        if(index_inside_char == 8){
            output_bits_as_char();
            index_inside_char = 0;
        }
    }
}

/*
 * Scrhijf de bits uit naar stdout als een char
 */
void output_bits_as_char() {
    fwrite(&char_for_stdout , 1, sizeof(char_for_stdout) , stdout);
}

/*
 * Code die steeds de grootst mogelijke node neemt
 * */
//unsigned int get_max_len(unsigned int max_len, unsigned int mem_lim, char number_of_len_1){
//    if(number_of_len_1 == (char) 256)
//        return 1;
//
//    size_t heap = get_heap_size();
//    unsigned int stacks = 7 * 1024; // stack size
//    unsigned int extra = 5 * 1024;
//    unsigned int total_available = mem_lim - heap - stacks - extra;
//    unsigned int total_needed_for_len_1 = (256 - number_of_len_1) * sizeof(Node) + (256 - number_of_len_1) * sizeof(char); // 256 characters
//    if(total_available > total_needed_for_len_1) {
//        int k = 0;
//        while (max_len - k != 0){
//            if((max_len - k) * sizeof(Node) < (total_available - total_needed_for_len_1))
//                return max_len - k;
//            k++;
//        }
//    }
//    return 1;
//}

/*
 * Bepaalt hoe lang het label mag zijn, rekening houdend met de geheugen limiet
 */
unsigned int get_max_len(char* buffer, unsigned int i, unsigned int a, unsigned int max_len, unsigned int mem_lim, char number_of_len_1){
    if(number_of_len_1 == 256)
        return 1;

    size_t heap = get_heap_size();
    unsigned int stacks = 7 * 1024; // stack size
    unsigned int extra = 5 * 1024;
    unsigned int total_available = mem_lim - heap - stacks - extra;
    unsigned int total_needed_for_len_1 = (256 - number_of_len_1) * sizeof(Node) + (256 - number_of_len_1) * sizeof(char); // 256 characters

    unsigned int max = i + max_len;
    if(max > a)
        max = a - i;

    if(total_available > total_needed_for_len_1) {
        unsigned int k = i;
        while(k != max){
            if(isspace(buffer[k]) != 0){
                if(k - i == 0){
                    return 1;
                } else {
                    // spatie niet meegerekend
                    return k - i;
                }
            }
            k++;
        }
    }
    return 1;
}

/*
 * Krijg de geheugenlimiet (2de paramater bij encoding)
 */
unsigned int get_memory_limit(unsigned int mem_lim){
    unsigned int mem_limits[10] = { 75 * 1024,
                                    75 * 1024,
                                    100 * 1024,
                                    200 * 1024,
                                    1024 * 1024,
                                    20 * 1024 * 1024,
                                    50 * 1024 * 1024,
                                    100 * 1024 * 1024,
                                    500 * 1024 * 1024,
                                    1024 * 1024 * 1024 };
    return mem_limits[mem_lim];

}