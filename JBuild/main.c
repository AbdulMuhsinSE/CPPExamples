#include <time.h>
#include "bst.h"
#include "fiq.h"

void *memAlloc(void *);
void *genStudent(void *);
void *remStudent(void *);

TreeNode *root;

int main(int argc, const char * argv[]) {
    
    
    srand(time(NULL));
    fiq_init();
    root = bst_init();
    
    pthread_t Th1, Th2, Th3;
    
    pthread_create(&Th1, NULL, memAlloc, NULL);
    pthread_create(&Th2, NULL, genStudent, NULL);
    pthread_create(&Th3, NULL, remStudent, NULL);
    
    pthread_join(Th1, NULL);
    pthread_join(Th2, NULL);
    pthread_join(Th3, NULL);

    
    
    
    return 0;
}

void *memAlloc(void *args){
    if(getItemsCount() == 0){
        allocMem(100);
        for(int i=0; i<100; i++){
            Data *temp = (Data *)malloc(sizeof(Data));
            enqueue(temp);
        }
    }
    
    long millis = 0;
    int threshold = 600;
    clock_t start, interval;
    start = clock();
    
    while(1){
        if(millis > threshold){
            int freeItems = getItemsCount();
            
            if(freeItems < 20){
                allocMem(50);
                freeItems += 50;
                for(int i=0; i<50; i++){
                    Data *temp = (Data *)malloc(sizeof(Data));
                    enqueue(temp);
                }
            }
            printf("%d Free Items left.",freeItems);
            printf("\n");
            
            start = clock();
        }
        
        interval = clock() - start;
        millis = interval * 1000 / CLOCKS_PER_SEC;
    }
}

void *genStudent(void *args){
    long millis = 0;
    int threshold = 200;
    clock_t start, interval;
    start = clock();
    
    while(1){
        if(millis > threshold){
            int sID = 0, temp = 0;
            bool unique = false;
            while(!unique){
                temp = rand() % (20000+1-10001)+10001;
                if(search(root, temp) == NULL){
                    //printf("unique\n");
                    //printf("%d\n",temp);
                    sID = temp;
                    unique = true;
                }
                else{
                    //printf("!unique");
                    unique = false;
                }
            }
            
            int age = rand() % (25+1-18)+18;
            Node *freeItem = NULL;
            freeItem = dequeue();
            Data *data = freeItem->data;
            data->sID=sID;
            data->age=age;
            root = insert(root, data);

            //printf("%d\n",sID);
            //printf("%d",age);
            //printf("\n");
            
            start = clock();
        }
        
        interval = clock() - start;
        millis = interval * 1000 / CLOCKS_PER_SEC;
    }
}

void *remStudent(void *args){
    long millis = 0;
    int threshold = 800;
    clock_t start, interval;
    start = clock();
    
    while(1){
        if(millis > threshold){
            if(root != NULL){
                root = delete(root, root->data);
            }
            Data *tmp = (Data *)malloc(sizeof(Data));
            enqueue(tmp);
            
            start = clock();
        }
        
        interval = clock() - start;
        millis = interval * 1000 / CLOCKS_PER_SEC;
    }
}