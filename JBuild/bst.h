#ifndef bst_h
#define bst_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "data.h"

typedef struct TreeNode{
    Data *data;
    struct TreeNode *right;
    struct TreeNode *left;
    pthread_mutex_t lock;
}TreeNode;

typedef struct Tree{
    TreeNode *root;
    pthread_mutex_t lock;
}Tree;

TreeNode * bst_init();
TreeNode * bst_getNewNode();
//void bst_init();
void print_preorder(TreeNode *node);
TreeNode * insert(TreeNode *node, Data *data);
TreeNode * search(TreeNode *node, int val);
TreeNode * findMin(TreeNode *node);
TreeNode * delete(TreeNode *node, Data *data);

#endif /* bst_h */
