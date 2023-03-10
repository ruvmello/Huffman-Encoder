//
// Created by ruben on 24-10-2021.
//

#include "tree.h"
#include <string.h>
#include <stdlib.h>

#define EXIT_MALLOC_FAILED 7

size_t heap_size = 0;
/*
 * Code van Ufora (Robin De Baets), uit de opgave
 */
void *safe_malloc(size_t __size) {
    void *result = malloc(__size);
    if (result == NULL) {
        exit(EXIT_MALLOC_FAILED);
    }
    heap_size += __size;
    return result;
}

/*
 * Maak nng node aan
 */
Node* krijg_nng(){
    Node* nng;
    nng = (Node*) safe_malloc(sizeof(Node));
    nng->gewicht = 0;
    nng->ordenummer = 1;
    nng->left_child = NULL;
    nng->right_child = NULL;
    nng->parent = NULL;

    return nng;
}

/*
 * Zoek de node met hetzelfde label in de boom
 */
Node *vind_node(Node *root, char* label, unsigned int size, bool search_nng) {
    if (root->left_child == NULL && root->right_child == NULL) { // blad

        if (root->ordenummer == 1) { // Ordenummer nng is altijd 1
            return root;
        } else if (!search_nng && root->label != NULL && memcmp(root->label, label, size) == 0 && root->size == size) { // als blad het label is, return
            return root;
        } else {
            return NULL; // anders return NULL
        }
    }

    Node *right = vind_node(root->right_child, label, size, search_nng);

    if(right != NULL && right->ordenummer != 1 && !search_nng){
        return right;
    } else if(right != NULL && search_nng && right->ordenummer == 1){
        return right;
    }

    Node *left = vind_node(root->left_child, label, size, search_nng);

    if (right == NULL && left == NULL) {
        return NULL;
    } else if (right == NULL) { // Left niet NULL
        return left;
    } else if (left == NULL) { // right niet NULL
        return right;
    } else {
        // Indien beide niet NULL zijn, is er 1 van de 2 nng.
        // In dat geval return degene die niet nng is.
        if (right->ordenummer == 1) {
            return left;
        } else {
            return right;
        }
    }
}

/*
 * Zoek de node met hetzelfde gewicht en hoogste orde nummer
 * als gegeven node t
 */
Node* vind_node_met_zelfde_gewicht_ordenummer_maximaal(Node* t, Node* root){
    Node* t_ster = t;

    if(root->gewicht < t->gewicht){
        return t_ster;
    }

    if(root->gewicht == t_ster->gewicht){
        if(root->ordenummer > t_ster->ordenummer){
            t_ster = root;
        }
    }

    Node* left = NULL;
    if(root->left_child != NULL){
        left = vind_node_met_zelfde_gewicht_ordenummer_maximaal(t_ster, root->left_child);
    }
    Node* right = NULL;
    if(root->right_child != NULL) {
        right = vind_node_met_zelfde_gewicht_ordenummer_maximaal(t_ster, root->right_child);
    }

    if(left != NULL && left->gewicht == t_ster->gewicht){
        if(left->ordenummer > t_ster->ordenummer){
            t_ster = left;
        }
    }

    if(right != NULL && right->gewicht == t_ster->gewicht){
        if(right->ordenummer > t_ster->ordenummer){
            t_ster = right;
        }
    }

    return t_ster;

}

/*
 * Maak kleine boom van 3 nodes en een label
 */
Node* maak_kleine_boom(char* label, Node* nng, unsigned int len){

    Node* nng2 = krijg_nng();
    Node* right = (Node*) safe_malloc(sizeof(Node));

    nng->ordenummer = 3;
    nng->gewicht = 1;
    nng->left_child = nng2;
    nng->right_child = right;

    right->parent = nng;
    right->ordenummer = 2;
    right->gewicht = 1;
    right->label = (char*) safe_malloc(sizeof(char) * len); // Size 1 character
    strncpy(right->label, label, len);
    right->size = len;
    right->left_child = NULL;
    right->right_child = NULL;

    nng2->parent = nng;

    return nng;
}

/*
 * Verhoog alle andere orde nummers met 2
 */
void verhoog_ordenummer_met_2(Node* root){
    if(root->left_child != NULL && root->left_child->gewicht != 0){
        root->ordenummer = root->ordenummer + 2;
        verhoog_ordenummer_met_2(root->right_child);
        verhoog_ordenummer_met_2(root->left_child);
    } else if(root->left_child == NULL && root->right_child == NULL){
        root->ordenummer = root->ordenummer + 2;
    }
}

/*
 * Het aanpassen van de boom nadat een nieuw character is ingelezen
 */
void pas_boom_aan(char* label, Node* root, Node* z, unsigned int len){
    Node* t;
    if(z->ordenummer != 1){ // z in boom
        t = z;
    } else {
        Node* o = maak_kleine_boom(label, z, len);
        verhoog_ordenummer_met_2(root);
        if(o->parent == NULL){
            return;
        } else {
            t = o->parent;
        }
    }

    while(t->parent != NULL){
        Node* t_ster = vind_node_met_zelfde_gewicht_ordenummer_maximaal(t, root);
        if(t->parent != t_ster){
            // wissel t en t' in de boom
            Node* swap_t = t->parent; // Ouder van t
            Node* swap_ster = t_ster->parent; // Ouder van t_ster

            if(swap_t->left_child == t){

                if(swap_ster->left_child == t_ster){
                    swap_ster->left_child = t;
                } else {
                    swap_ster->right_child = t;
                }

                swap_t->left_child = t_ster;
            } else {

                if(swap_ster->left_child == t_ster){
                    swap_ster->left_child = t;
                } else {
                    swap_ster->right_child = t;
                }

                swap_t->right_child = t_ster;
            }

            t_ster->parent = swap_t;
            t->parent = swap_ster;

            //wissel de ordenummers
            int swap_orde = t->ordenummer;
            t->ordenummer = t_ster->ordenummer;
            t_ster->ordenummer = swap_orde;
        }
        t->gewicht = t->gewicht + 1;
        t = t->parent;
    }

    // wortel
    t->gewicht = t->gewicht + 1;
}

/*
 * Op het einde van het programma, geef alle nodes vrij
 */
void clean_up_tree(Node* root){
    // Recursief free toepassen
    if(root->left_child != NULL)
        clean_up_tree(root->left_child);

    if(root->right_child != NULL)
        clean_up_tree(root->right_child);

    // Als label niet NULL, ook free
    if(root->label != NULL)
        free(root->label);

    // Tot slot, free de node zelf
    free(root);
}

/*
 * Geef de grootte van de heap terug
 */
size_t get_heap_size(){
    return heap_size;
}
