//
// Created by ruben on 17-10-2021.
//

#ifndef PROJECT_ENCODER_H
#define PROJECT_ENCODER_H

#include<stdbool.h>
#include "tree.h"

void encode(unsigned int max_len, unsigned int lim);

void output_of_encoding(Node* node, Node* root, char* label, unsigned int size, bool zit_in_boom);
void output_of_pad(Node* node, Node* root);
void output_bits_as_char();
//unsigned int get_max_len(unsigned int max_len, unsigned int mem_lim, char number_of_len_1);
unsigned int get_max_len(char* buffer, unsigned int i, unsigned int a, unsigned int max_len, unsigned int mem_lim, char number_of_len_1);
unsigned int get_memory_limit(unsigned int mem_lim);

#endif //PROJECT_ENCODER_H
