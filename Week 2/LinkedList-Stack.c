#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

struct node {
    int data;
    struct node *next;
};

struct node* createNode(int data) {
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void printMenu() {
    printf("Linked List X Stack\n");
    printf("======================\n");
    printf("1. Push Data\n");
    printf("2. Pop Data\n");
    printf("3. View Data\n");
    printf("4. Exit\n");
    printf("======================\n");
    printf("Input : ");
}

void push(struct node** head) {
    int newData;
    printf("Input new data: ");
    scanf("%d", &newData);

    struct node* newNode = createNode(newData);

    if (*head == NULL) {
        *head = newNode;
    } else {
        newNode->next = *head;
        *head = newNode;
    }
}

void pop(struct node** head) {
    if (*head == NULL) {
        printf("Stack is empty\n");
    } else {
        struct node* currHead = *head;
        *head = (*head)->next;
        free(currHead);
    }
}

void view(struct node* head) {
    if (head == NULL) {
        printf("No data in stack\n");
    } else {
        struct node* currNode = head;
        int i = 1;

        while (currNode != NULL) {
            printf("Data %d: %d\n", i, currNode->data);
            currNode = currNode->next;
            i++;
        }
    }
}

int main() {
    struct node* head = NULL;
    int inputMenu = 0;

    do {
        system("cls");
        printMenu();
        scanf("%d", &inputMenu);

        switch (inputMenu) {
            case 1:
                push(&head); // push
                break;
            case 2:
                pop(&head); // pop
                break;
            case 3:
                view(head); // view
                break;
            case 4:
                printf("Thank you!\n");
                break;
            default:
                printf("Error input! Try again\n");
        }

        printf("Press any button to continue\n");
        getch();

    } while (inputMenu != 4);

    return 0;
}
