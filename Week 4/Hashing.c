#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 26

struct node {
    char name[50];
    char phoneNumber[15];
    struct node *next;
};

struct node *hashTable[SIZE];

struct node* createNode(char *newName, char *newPhoneNumber) {
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    
    strcpy(newNode->name, newName);
    strcpy(newNode->phoneNumber, newPhoneNumber);
    newNode->next = NULL;
    
    return newNode;
}

int getHash(char *name) {
    int firstCharAscii = (int)(toupper(name[0]) - 'A');
    return firstCharAscii % SIZE;
}

void insert() {
    char newName[50];
    char newPhoneNumber[15];
    
    printf("Input name: ");
    scanf("%s", newName);
    printf("Input phone number: ");
    scanf("%s", newPhoneNumber);
    
    int indexTable = getHash(newName);
    
    struct node *newNode = createNode(newName, newPhoneNumber);
    newNode->next = hashTable[indexTable];
    hashTable[indexTable] = newNode;
    
    printf("Data inserted successfully!\n");
}

void viewAll() {
    int i;
    for (i = 0; i < SIZE; i++) {
        if (hashTable[i] != NULL) {
            printf("Data in index %d:\n", i);
            struct node* currNode = hashTable[i];
            while (currNode != NULL) {
                printf("%s - %s\n", currNode->name, currNode->phoneNumber);
                currNode = currNode->next;
            }
        }
    }
}

void del() {
    char nameToDelete[50];
    printf("Input name to be deleted: ");
    scanf("%s", nameToDelete);
    
    int indexTable = getHash(nameToDelete);
    
    if (hashTable[indexTable] == NULL) {
        printf("There is no data in this index.\n");
    } else {
        struct node *prevNode = NULL;
        struct node *currNode = hashTable[indexTable];
        
        while (currNode != NULL) {
            if (strcmp(currNode->name, nameToDelete) == 0) {
                if (prevNode == NULL) {
                    hashTable[indexTable] = currNode->next;
                } else {
                    prevNode->next = currNode->next;
                }
                free(currNode);
                printf("Data %s has been deleted.\n", nameToDelete);
                return;
            }
            prevNode = currNode;
            currNode = currNode->next;
        }
        printf("Data %s not found.\n", nameToDelete);
    }
}

void printMenu() {
    printf("Hash Table\n");
    printf("==================\n");
    printf("1. Insert new data\n");
    printf("2. View all data\n");
    printf("3. Delete data\n");
    printf("4. Exit\n");
    printf("==================\n");
    printf(">> ");
}

int main() {
    int inputMenu = 0;
    
    for (int i = 0; i < SIZE; i++) {
        hashTable[i] = NULL;
    }
    
    do {
        system("cls");
        printMenu();
        scanf("%d", &inputMenu);
        
        switch (inputMenu) {
            case 1:
                insert();
                break;
            case 2:
                viewAll();
                break;
            case 3:
                del();
                break;
            case 4:
                printf("Thank you...\n");
                break;
            default:
                printf("Invalid input! Please enter a number between 1 and 4.\n");
        }
        
        if (inputMenu != 4) {
            printf("Press any key to continue...\n");
            getchar();
            getchar();
        }
    } while (inputMenu != 4);
    
    return 0;
}
