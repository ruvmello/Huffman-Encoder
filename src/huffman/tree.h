//
// Created by ruben on 24-10-2021.
//

#ifndef PROJECT_TREE_H
#define PROJECT_TREE_H

#include <stddef.h>
#include "stdbool.h"

typedef struct Node {
    char* label;
    unsigned int gewicht; // a(x)
    unsigned int ordenummer; // o(x)

    unsigned int size;

    struct Node* parent;

    struct Node* left_child;
    struct Node* right_child;
} Node;

Node* krijg_nng();

Node *vind_node(Node *root, char* label, unsigned int size, bool search_nng);
Node* vind_node_met_zelfde_gewicht_ordenummer_maximaal(Node* t, Node* root);

Node* maak_kleine_boom(char* label, Node* nng, unsigned int len);

void verhoog_ordenummer_met_2(Node* root);

void pas_boom_aan(char* label, Node* root, Node* z, unsigned int len);

void clean_up_tree(Node* root);

size_t get_heap_size();

#endif //PROJECT_TREE_H
