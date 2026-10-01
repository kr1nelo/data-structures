#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CAKES 10

struct cake{
    int no;
    char code[6];
    char name[50];
    int available;
    float price;
};

struct cake cakes[MAX_CAKES] = {
    {1, "CK001", "Chocolate Cake", 10, 50000},
    {2, "CK002", "Strawberry Cake", 15, 60000},
    {3, "CK003", "Vanilla Cake", 20, 45000},
    {4, "CK004", "Cheese Cake", 5, 70000},
    {5, "CK005", "Red Velvet Cake", 8, 65000}
};

int cake_count = 5;

void displayCakes(){
    printf("No\tCode\tName\t\t\tAvailable\tPrice\n");
    for(int i = 0; i < cake_count; i++){
        printf("%d\t%s\t%s\t\t%d\t\tRp%.2f\n", cakes[i].no, cakes[i].code, cakes[i].name, cakes[i].available, cakes[i].price);
    }
}

void sellCake(){
    char code[6];
    int qty;

    printf("Enter Cake Code: ");
    scanf("%s", code);

    int found = 0;
    for(int i = 0; i < cake_count; i++){
        if(strcmp(cakes[i].code, code) == 0){
            found = 1;
            printf("Enter Quantity: ");
            scanf("%d", &qty);

            if(qty > 0 && qty <= cakes[i].available) {
                float totalPrice = cakes[i].price * qty;
                printf("Total Price: Rp%.2f\n", totalPrice);
                cakes[i].available -= qty;
                printf("Sell Success\n");
            } else {
                printf("The quantity of cake is not enough\n");
            }
            break;
        }
    }

    if(!found){
        printf("The Cake Code doesn't exist\n");
    }
}

void addStock(){
    char code[6];
    int qty;

    printf("Enter Cake Code: ");
    scanf("%s", code);

    int found = 0;
    for(int i = 0; i < cake_count; i++){
        if(strcmp(cakes[i].code, code) == 0){
            found = 1;
            printf("Enter Quantity (1-10): ");
            scanf("%d", &qty);

            if(qty >= 1 && qty <= 10) {
                cakes[i].available += qty;
                printf("Adding Stock Success\n");
            } else {
                printf("Invalid Quantity\n");
            }
            break;
        }
    }

    if(!found){
        printf("The Cake Code doesn't exist\n");
    }
}

int main(){
    int choice;

    do{
    	system("cls");
        printf("BLUE Cake Shop\n");
        printf("==================\n");
        printf("1. Sell\n");
        printf("2. Add Stock\n");
        printf("3. Exit\n");
        printf("==================\n");
        printf("Input: ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
            	system("cls");
                displayCakes();
                sellCake();
                break;
            case 2:
            	system("cls");
                displayCakes();
                addStock();
                break;
            case 3:
                printf("Exit the program\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
                system("cls");
        }
    } while(choice != 3);

    return 0;
}
