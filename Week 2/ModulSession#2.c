#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char medicines[50][50];
    int top;
}Stack;

void initStack(Stack *s){
    s->top = -1;
}

int isFull(Stack *s){
    return s->top == 49;
}

int isEmpty(Stack *s){
    return s->top == -1;
}

void push(Stack *s, char *medicine){
    if(isFull(s)){
        printf("Stack is full. Cannot add more medicines\n");
        return;
    }
    
    strcpy(s->medicines[++s->top], medicine);
    printf("Medicine %s added to prescription.\n", medicine);
}

void pop(Stack *s){
    if(isEmpty(s)){
        printf("Stack is empty. No medicines to take\n");
        return;
    }
    
    printf("Taking medicine: %s\n", s->medicines[s->top--]);
}

void display(Stack *s){
    if(isEmpty(s)){
        printf("No medicines in prescription\n");
        return;
    }
    
    printf("Medicine Prescription:\n");
    int count = 1;
    for(int i = s->top; i >= 0; i--) {
        printf("Take Medicine #%d: %s\n",count ,s->medicines[i]);
        count++;
    }
}

int main(){
    Stack s;
    initStack(&s);
    char medicine[50];
    char choice[10];
    
    printf("Sunib Hospital\n");
    printf("===============\n");
    printf("\n");
    printf("\n");

    while(1){
    	printf("Enter medicine name [minimum 5 medicines] ('done' to finish): ");
        scanf(" %49[^\n]", medicine);

        if(strcasecmp(medicine, "done") == 0){
            break;
        }

        push(&s, medicine);
    }

    display(&s);

    printf("Do you want to continue? (yes/no): ");
    scanf(" %9s", choice);

    if(strcasecmp(choice, "yes") == 0){
        pop(&s);
    }

    return 0;
}
