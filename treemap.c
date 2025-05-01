#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;

    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;

    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

    Pair *insertar = searchTreeMap(tree, key);
    if(insertar != NULL && is_equal(tree, key, insertar->key)) return; //no se inserta si ya existe

    if(tree->lower_than(key, tree->current->pair->key)) 
    {
        tree->current->left = createTreeNode(key, value);
        tree->current->left->parent = tree->current;
        tree->current = tree->current->left;
    } 
    else 
    {
        tree->current->right = createTreeNode(key, value);
        tree->current->right->parent = tree->current;
        tree->current = tree->current->right;
    }

}

TreeNode * minimum(TreeNode * x){

    while(x->left != NULL) {
        x = x->left;
    }

    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

    if(node->left == NULL && node->right == NULL) 
    {
        if(node->parent) {
            if(node->parent->left == node) node->parent->left = NULL;
            else node->parent->right = NULL;

        } 
        else tree->root = NULL;

        free(node->pair);
        free(node);
    } 
    else if(node->left == NULL || node->right == NULL)
    {
        TreeNode * child = (node->left) ? node->left : node->right; //se busca el hijo que tiene el nodo a eliminar
        if(node->parent) {
            if(node->parent->left == node) node->parent->left = child;
            else node->parent->right = child;
        } 
        else tree->root = child;

        child->parent = node->parent;

        free(node->pair);
        free(node);
    } 
    else 
    {
        TreeNode * successor = minimum(node->right);
        node->pair = successor->pair;
        removeNode(tree, successor);
    }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {

    if(tree == NULL || tree->root == NULL) return NULL;

    tree->current = tree->root;

    while(!is_equal(tree, key, tree->current->pair->key)) {
        if (tree->lower_than(key, tree->current->pair->key)) {
            if (tree->current->left == NULL) return NULL;
            tree->current = tree->current->left;
        } else {
            if (tree->current->right == NULL) return NULL;
            tree->current = tree->current->right;
        }
    }

    return tree->current->pair;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {

    if(tree == NULL || tree->root == NULL) return NULL;
    tree->current = minimum(tree->root);
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if(tree == NULL || tree->root == NULL || tree->current == NULL) return NULL;

    if (tree->current->right) tree->current = minimum(tree->current->right);
    else if(tree->current->parent) tree->current = tree->current->parent;
        /*if(tree->current->parent->left == tree->current)
        {
            if(tree->current->parent->right)
            {
                tree->current = minimum(tree->current->parent->right);
            }
            else tree->current = tree->current->parent;
        }
        else tree->current->parent->right = NULL;*/

    return (tree->current) ? tree->current->pair : NULL;
}
