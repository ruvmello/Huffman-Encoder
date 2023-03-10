//
// Created by ruben on 3-12-2021.
//
#include "../huffman/tree.h"
#include "stdio.h"
#include <assert.h>
#include "string.h"

/*
 * Is de initiele node gelijk aan nng
 * */
Node* test1(){
    Node* root = krijg_nng();
    assert(root->ordenummer == 1 && root->gewicht == 0);
    assert(root->left_child == NULL && root->right_child == NULL && root->parent == NULL);
    return root;
}

/*
 * klopt de kleine boom
 * */
Node* test2(Node* root){
    char* label = "a";
    Node* small_tree = maak_kleine_boom(label, root, 1);
    assert(small_tree->ordenummer == 3 && small_tree->left_child->ordenummer == 1 && small_tree->right_child->ordenummer == 2);
    assert(small_tree->gewicht == 1 && small_tree->right_child->gewicht == 1 && small_tree->left_child->gewicht == 0);
    assert(strncmp(small_tree->right_child->label, label, 1) == 0);
    return small_tree;
}

/*
 * Pas boom aan met een nieuw character
 * */
void test3(Node* root){
    char* label2 = "a";
    char* label = "b";
    pas_boom_aan(label, root, root->left_child, 1);
    assert(strncmp(root->right_child->label, label2, 1) == 0);
    assert(strncmp(root->left_child->right_child->label, label, 1) == 0);
    assert(root->ordenummer == 5);
    assert(root->gewicht == 2);
}

/*
 * vind_node_met_zelfde_gewicht_orde_nummer_maximaal
 * */
void test4(Node* root){
    Node* gevonden_node = vind_node_met_zelfde_gewicht_ordenummer_maximaal(root->right_child, root);
    // Moet zichzelf zijn
    assert(gevonden_node == root->right_child);

    gevonden_node = vind_node_met_zelfde_gewicht_ordenummer_maximaal(root->left_child->right_child, root);
    // Moet root->right zijn
    assert(gevonden_node == root->right_child);
}

/*
 * verhoog alle ordenummers met 2
 * */
void test5(Node* root){
    verhoog_ordenummer_met_2(root);
    assert(root->ordenummer == 7 && root->right_child->ordenummer == 6);
    // vanaf ordenummer 3 moet recursie niet verder
    assert(root->left_child->ordenummer == 3);
    // reset ordenummers
    root->ordenummer = 5;
    root->right_child->ordenummer = 4;
}

/*
 * vind nng
 * */
void test6(Node* root){
    Node* z = vind_node(root, "c", 1, true);
    assert(z->ordenummer == 1);
}

/*
 * vind bestaande node
 * */
void test7(Node* root){
    Node* z = vind_node(root, "b", 1, false);
    assert(strncmp(z->label, "b", 1) == 0);
}

/*
 * Pas boom aan met een bestaand character
 * */
void test8(Node* root){
    Node* z = vind_node(root, "b", 1, false);
    char* label = "b";
    pas_boom_aan(label, root, z, 1);
    // b moet nu verschoven zijn
    assert(strncmp(root->right_child->label, label, 1) == 0);
    assert(root->right_child->gewicht == 2);
}

/*
 * Voer de tests uit
 */
void run_tests(){
    Node* root = test1();
    printf("TEST 1: GESLAAGD\n");
    root = test2(root);
    printf("TEST 2: GESLAAGD\n");
    test3(root);
    printf("TEST 3: GESLAAGD\n");
    test4(root);
    printf("TEST 4: GESLAAGD\n");
    test5(root);
    printf("TEST 5: GESLAAGD\n");
    test6(root);
    printf("TEST 6: GESLAAGD\n");
    test7(root);
    printf("TEST 7: GESLAAGD\n");
    test8(root);
    printf("TEST 8: GESLAAGD\n");

    printf("ALLE TESTEN GESLAAGD");
}
