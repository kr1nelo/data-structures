#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Patient{
    char name[26];
    int age;
    char symptoms[100];
    char code[10];
    int priority;
    struct Patient* next;
    struct Patient* prev;
} Patient;

typedef struct{
    Patient* head;
    Patient* tail;
} PriorityQueue;

void insPatient(PriorityQueue* pq, char name[], int age, char symptoms[], char code[]){
    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    strcpy(newPatient->name, name);
    newPatient->age = age;
    strcpy(newPatient->symptoms, symptoms);
    strcpy(newPatient->code, code);

    if(strcmp(code, "Red") == 0){
        newPatient->priority = 3;
    } else if(strcmp(code, "Yellow") == 0){
        newPatient->priority = 2;
    } else if(strcmp(code, "Green") == 0){
        newPatient->priority = 1;
    }

    newPatient->next = NULL;
    newPatient->prev = NULL;

    if(pq->head == NULL){
        pq->head = pq->tail = newPatient;
    } else{
        Patient* current = pq->head;
        while(current != NULL && current->priority >= newPatient->priority){
            current = current->next;
        }

        if(current == NULL){
            newPatient->prev = pq->tail;
            pq->tail->next = newPatient;
            pq->tail = newPatient;
        } else if(current->prev == NULL){
            newPatient->next = pq->head;
            pq->head->prev = newPatient;
            pq->head = newPatient;
        } else{
            newPatient->next = current;
            newPatient->prev = current->prev;
            current->prev->next = newPatient;
            current->prev = newPatient;
        }
    }
}

void viewQueue(PriorityQueue* pq){
    if(pq->head == NULL){
    	printf("There is no queue yet !\n");
    	printf("");
    } else{
        Patient* current = pq->head;
        while(current != NULL){
            printf("Name: %s, Age: %d, Symptoms: %s, Code: %s\n", current->name, current->age, current->symptoms, current->code);
            current = current->next;
        }
    }
}

void nextQueue(PriorityQueue* pq){
    if(pq->head == NULL){
        printf("There is no queue yet!\n");
        printf("");
    } else{
        Patient* temp = pq->head;
        printf("The next patient is: \n");
		printf("Name: %s \n" , temp->name); 
		printf("Age: %d \n", temp->age);
		printf("Description: %s\n", temp->symptoms);
		printf("Code: %s\n", temp->code);
        pq->head = pq->head->next;
        if(pq->head != NULL){
            pq->head->prev = NULL;
        } else{
            pq->tail = NULL;
        }
        free(temp);
    }
}

int main() {
    PriorityQueue pq = {NULL, NULL};
    int choice;
    char name[26];
    char symptoms[100];
    char code[10];
    int age;

    while (1) {
        printf("Bluejack Hospital\n");
        printf("=================\n");
        printf("1. Insert\n");
        printf("2. View\n");
        printf("3. Next Queue\n");
        printf("4. Exit\n");
        printf(">> ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
            	system("cls");
                while(1){
                    printf("Input patient name[4-25]: ");
                    scanf(" %[^\n]", name);
                    if (strlen(name) >= 4 && strlen(name) <= 25){
                        break;
                    } else{
                        printf("");
                    }
                }

                while(1){
                    printf("Input patient age[>= 0]: ");
                    scanf("%d", &age);
                    if(age >= 0){
                        break;
                    } else{
                        printf("");
                    }
                }

                while(1){
                    printf("Input description[>= 6 characters]: ");
                    scanf(" %[^\n]", symptoms);
                    if (strlen(symptoms) >= 6){
                        break;
                    } else{
                        printf("");
                    }
                }

                while(1){
                    printf("Input code [Red|Yellow|Green]: ");
                    scanf(" %[^\n]", code);
                    if (strcmp(code, "Red") == 0 || strcmp(code, "Yellow") == 0 || strcmp(code, "Green") == 0){
                        break;
                    } else{
                        printf("");
                    }
                }

                insPatient(&pq, name, age, symptoms, code);
                printf("Insert success !\n");
                getchar();
                getchar();
                system("cls");
                break;

            case 2:
            	system("cls");
                viewQueue(&pq);
                printf("Press Enter to continue ...\n");
                getchar();
                getchar();
                system("cls");
                break;

            case 3:
            	system("cls");
                nextQueue(&pq);
                printf("Press Enter to continue ...\n");
                getchar();
                getchar();
                system("cls");
                break;

            case 4:
            	system("cls");
                printf("Exiting program...\n");
                return 0;

            default:
                printf("Invalid choice! Please choose between 1 and 4.\n");
        }
    }

    return 0;
}
