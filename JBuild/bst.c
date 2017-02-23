#include "bst.h"

Tree *tree = NULL;

TreeNode * bst_init(){
    tree = (Tree *)malloc(sizeof(Tree));
    tree->root = NULL;
    pthread_mutex_t temp = PTHREAD_MUTEX_INITIALIZER;
    tree->lock = temp;
    return tree->root;
}

TreeNode * bst_getNewNode(){
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    pthread_mutex_t temp = PTHREAD_MUTEX_INITIALIZER;
    node->lock = temp;
    node->right = NULL;
    node->left = NULL;
    return node;
}

void print_preorder(TreeNode *node){
    
    if(node != NULL){
        printf("%d",node->data->sID);
        print_preorder(node->left);
        print_preorder(node->right);
    }
}

TreeNode * insert(TreeNode *node, Data *data){
    //printf("Insert\n");
    
    int error = pthread_mutex_lock(&tree->lock);
    if(error == 0){
        //printf("Locked\n");
    }
    else{
        //printf("Not Locked, Error: %d\n", error);
    }
    
    if(node == NULL){
        node = bst_getNewNode();
        node->data = data;
        
        error = pthread_mutex_unlock(&tree->lock);
        if(error == 0){
            //printf("Unlocked(NULL)\n");
        }
        else{
            //printf("Not Unlocked, Error: %d\n", error);
        }
        //printf("Print\n");
        //print_preorder(node);
        //printf("\n");
        return node;
    }
    
    TreeNode *temp = node;
    TreeNode *parent = NULL;
    char type = ' ';
    
    while(temp){
        parent = temp;
        if((node->data->sID) > data->sID){
            temp = temp->left;
            type = 'l';
        }
        else if((node->data->sID) < data->sID){
            temp = temp->right;
            type = 'r';
        }
    }
    
    temp = bst_getNewNode();
    temp->data = data;
    if(type == 'l'){
        parent->left = temp;
    }
    else{
        parent->right = temp;
    }
    //printf("Print\n");
    //print_preorder(node);
    //printf("\n");
    
    error = pthread_mutex_unlock(&tree->lock);
    if(error == 0){
        //printf("Unlocked\n");
    }
    else{
        //printf("Not Unlocked, Error: %d\n", error);
    }
    
    return node;
}

TreeNode * search(TreeNode *node, int val){
    //printf("Find\n");
    
    int error = pthread_mutex_lock(&tree->lock);
    if(error == 0){
        //printf("Locked\n");
    }
    else{
        //printf("Not Locked, Error: %d\n", error);
    }
    
    if(node == NULL){
        error = pthread_mutex_unlock(&tree->lock);
        if(error == 0){
            //printf("Unlocked\n");
        }
        else{
            //printf("Not Unlocked, Error: %d\n", error);
        }
        return NULL;
    }
    
    while(node != NULL && val != node->data->sID){
        if(val < node->data->sID){
            node = node->left;
        }
        else{
            node = node->right;
        }
    }
    
    error = pthread_mutex_unlock(&tree->lock);
    if(error == 0){
        //printf("Unlocked\n");
    }
    else{
        //printf("Not Unlocked, Error: %d\n", error);
    }
    return node;
    
    
}

TreeNode * findMin(TreeNode *node){
    //printf("Minimum\n");
    
    if(node == NULL){
        return NULL;
    }
    
    if(node->left == NULL){
        return node;
    }
    
    return findMin(node->left);
}

TreeNode * delete(TreeNode *node, Data *data){
    //printf("Delete\n");
    
    int error = pthread_mutex_lock(&tree->lock);
    if(error == 0){
        //printf("Locked\n");
    }
    else{
        //printf("Not Locked, Error: %d\n", error);
    }
    
    if(node == NULL){
        error = pthread_mutex_unlock(&tree->lock);
        if(error == 0){
            //printf("Unlocked\n");
        }
        else{
            //printf("Not Unlocked, Error: %d\n", error);
        }
        return node;
    }
    
    if(node == NULL && data == NULL){
        error = pthread_mutex_unlock(&tree->lock);
        if(error == 0){
            //printf("Unlocked\n");
        }
        else{
            //printf("Not Unlocked, Error: %d\n", error);
        }
        return node;
    }
    
    TreeNode *parent = NULL;
    TreeNode *temp = node;
    TreeNode *delParent = NULL;
    TreeNode *delNode = node;
    
    if(delNode->right == NULL){
        if(delParent == NULL){
            node = delNode->left;
            free(delNode);
            
            error = pthread_mutex_unlock(&tree->lock);
            if(error == 0){
                //printf("Unlocked\n");
            }
            else{
                //printf("Not Unlocked, Error: %d\n", error);
            }
            
            return node;
        }
        else{
            if(delParent->left == delNode)
                delParent->left = delNode->left;
            else
                delParent->right = delNode->left;
            free(delNode);
            
            error = pthread_mutex_unlock(&tree->lock);
            if(error == 0){
                //printf("Unlocked\n");
            }
            else{
                //printf("Not Unlocked, Error: %d\n", error);
            }
            
            return node;
        }
    }
    else{
        if(delNode->left == NULL){
            if(delParent == NULL){
                node = delNode->right;
                free(delNode);
                
                error = pthread_mutex_unlock(&tree->lock);
                if(error == 0){
                    //printf("Unlocked\n");
                }
                else{
                    //printf("Not Unlocked, Error: %d\n", error);
                }
                
                return node;
            }
            else{
                if(delParent->left == delNode)
                    delParent->left = delNode->right;
                else
                    delParent->right = delNode->right;
                free(delNode);
                
                error = pthread_mutex_unlock(&tree->lock);
                if(error == 0){
                    //printf("Unlocked\n");
                }
                else{
                    //printf("Not Unlocked, Error: %d\n", error);
                }
                
                return node;
            }
        }
        else{
            temp = delNode->left;
            parent = delNode;
            while(temp->right != NULL){
                parent = temp;
                temp = temp->right;
            }
            delNode->data = temp->data;
            
            if(parent == delNode)
                parent->left = temp->left;
            else
                parent->right = temp->left;
            free(temp);
            
            error = pthread_mutex_unlock(&tree->lock);
            if(error == 0){
                //printf("Unlocked\n");
            }
            else{
                //printf("Not Unlocked, Error: %d\n", error);
            }
            
            return node;
        }
    }
    
}