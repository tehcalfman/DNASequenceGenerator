#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//author: Niall Cowman

//define the Base struct
struct Base{
    char type;
};

//define the a Node
struct Node{
    struct Base base;
    struct Node *next;
    struct Node *prev;

    //info to store in all nodes
    int age;
    int max_life;
    int gro_prob; //growth probability
    int del_prob; //deletion probability
    int mut_prob; //mutation probability
};

struct Node* head;
struct Node* tail;

//define the functions

//define a function to generate a random type for the base
char randomType(){
    char typeList[4] = {'A','C','G','T'};
    int choice = rand() % 4;
    return typeList[choice];
}

//define a function to create a base
struct Base* createBase(char t){
    struct Base* base = (struct Base*)malloc(sizeof(struct Base));
    base->type = t;
    return base;
}

//define a function to create a node
struct Node* createNode(struct Base base){
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->base = base;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// a function to print a sequence
void printSequence(){
    struct Node* print = head;
    //printf("Sequence: ");
    while(print != NULL){
        printf("%c", print->base.type);
        print = print->next;
    }
    printf("\n");
}

//a function to create a string of the sequence
char createSequenceString(int strLength){
    char sequenceString[strLength];
    struct Node* print = head;
    int i = 0;
    //printf("Sequence: ");
    while(print != NULL){
        sequenceString[i] = print->base.type;
        print = print->next;
        i++;
    }
    printf("\n");
    return *sequenceString;
}

// a function to add to the end of the sequence
void addToSequence(struct Base base){
    struct Node* new_tail = createNode(base);
    if(tail == NULL){
        head = new_tail;
        tail = new_tail;
    }
    else{
        new_tail->prev = tail;
        tail->next = new_tail;
        tail = new_tail;
    }
}

// a function to add at specific point in the sequence
void insertAtPoint(struct Base base, int pos){
    if(head == NULL || pos <= 0){
        return;
    }
    struct Node* current = head;
    int i;
    for(i = 1; current != NULL && i < pos; i++){
        current = current->next;
    }
    if(current == NULL){
        return;
    }
    struct Node* insertedNode = createNode(base);

    if(current->prev != NULL){
        current->prev->next = insertedNode;
    }else{
        head = insertedNode;
    }

    insertedNode->prev = current->prev;
    insertedNode->next = current;
    current->prev = insertedNode;
}

//a function to delete a node
void deleteNode(struct Node* del){
    if(head == NULL || del == NULL){
        return;
    }
    if(head == del){
        head = del->next;
    }
    if(del->next != NULL){
        del->next->prev = del->prev;
    }
    if(del->prev != NULL){
        del->prev->next = del->next;
    }
    free(del);
}

//a function to delete a node at a specific position within the sequence
void delAtPos(int index){
    if(head == NULL || index <= 0){
        return;
    }
    struct Node* current = head;
    int i;
    for(i = 1; current != NULL && i < index; i++){
        current = current->next;
    }
    if(current == NULL){
        return;
    }
    deleteNode(current);
}

void editAtPos(int index, struct Base newBase){
    if(head == NULL || index <= 0){
        return;
    }
    struct Node* current = head;
    int i;
    for(i = 1; current != NULL && i < index; i++){
        current = current->next;
    }
    if(current == NULL){
        return;
    }
    current->base = newBase;
}

//function to search for patterns in the sequence
void patternSearch(int size){
    char newString = createSequenceString(size);
    if(strstr(&newString, "AAA") != NULL){
        head->max_life += 1;
    }
    if(strstr(&newString, "CCC") != NULL){
        head->max_life -= 1;
    }
    if(strstr(&newString, "GGG") != NULL){
        struct Node* current = head;
        int i = 1;
        while(current != NULL){
            if(i % 2 == 0){
                delAtPos(i);
            }
            current = current->next;
            i++;
        }
        if(current == NULL){
            return;
        }
    }
    if(strstr(&newString, "TTT") != NULL){
        struct Node* current = head;
        int i;
        for(i = 1; current != NULL && i < size; i++){
            addToSequence(*createBase(current->base.type));
            current = current->next;
        }
        if(current == NULL){
            return;
        }
    }
}

//define a function to start a sequence(doubly linked list)
void startSequence(struct Base base){
    struct Node* new_head = createNode(base);
    if(head == NULL){
        head = new_head;
        tail = new_head;
        new_head->age = 0;
        new_head->max_life = rand() % 43 + 8;
        new_head->gro_prob = 25;//use the random variable to call on every loop, to check the following
        new_head->del_prob = 25;
        new_head->mut_prob = 10;
        return;
    }
    else{
        printf("Sequence already exists :3");
        return;
    }
}



int main() {
    //creating a seeded random based on the time(this took me way longer than it should've to figure out [ ;-;])
    time_t t;
    srand((unsigned) time(&t));

    int maxSize = 20;
    //Lifecycle
    startSequence(*createBase(randomType()));
    int sequenceSize = 1;
    int i;

    for(i = 0;i < head->max_life && sequenceSize <= maxSize; i++){
        printf("i: %d\n", i);
        printf("Max life: %d\n", head->max_life);
        printf("Sequence size: %d\n", sequenceSize);

        int del = head->del_prob;
        int grow = head->gro_prob;
        int mutate = head->mut_prob;
        //printf("del: %d\n", del);
        //printf("grow: %d\n", grow);
        //printf("mutate: %d\n", mutate);


        int growRand = rand() % 100;
        if(growRand <= grow){
            if(sequenceSize == 1){
                printf("Grew\n");
                addToSequence(*createBase(randomType()));
                sequenceSize += 1;
            }else{
                printf("Grew\n");
                int insertPos = rand() % (sequenceSize - 2 + 1) + 2;
                insertAtPoint(*createBase(randomType()), insertPos);
                sequenceSize += 1;
            }
        }
        int delRand =  rand() % 100;
        if(delRand <= del){
            delAtPos(rand() % sequenceSize + 1);
            printf("Deleted\n");
            sequenceSize -= 1;
        }
        if(i == 5){
            head->mut_prob = 10;
        }
        int mutRand =  rand() % 100;
        if(mutRand <= mutate){
            int mutationPos = rand() % sequenceSize + 1;
            struct Base newBase = *createBase(randomType());
            editAtPos(mutationPos, newBase);
            printf("Mutation\n");
        }
        if(i > 5){
            head->mut_prob += 1;
        }
        patternSearch(sequenceSize);

        head->age++;
        if(sequenceSize == 0){
            printf("Sequence was too weak to survive\n");
            break;
        }
        printSequence();
    }
    printf("Final sequence: ");
    printSequence();

    return 0;
}