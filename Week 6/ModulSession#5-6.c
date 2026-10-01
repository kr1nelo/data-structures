#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char phone[14];
    char name[26];
    char email[21];
    int points;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(char phone[], char name[], char email[], int points) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->phone, phone);
    strcpy(newNode->name, name);
    strcpy(newNode->email, email);
    newNode->points = points;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, char phone[], char name[], char email[], int points) {
    if (root == NULL) return createNode(phone, name, email, points);
    if (strcmp(phone, root->phone) < 0)
        root->left = insert(root->left, phone, name, email, points);
    else if (strcmp(phone, root->phone) > 0)
        root->right = insert(root->right, phone, name, email, points);
    return root;
}

Node* search(Node* root, char phone[]) {
    if (root == NULL || strcmp(root->phone, phone) == 0)
        return root;
    if (strcmp(phone, root->phone) < 0)
        return search(root->left, phone);
    return search(root->right, phone);
}

Node* deleteNode(Node* root, char phone[]) {
    if (root == NULL) return root;
    if (strcmp(phone, root->phone) < 0)
        root->left = deleteNode(root->left, phone);
    else if (strcmp(phone, root->phone) > 0)
        root->right = deleteNode(root->right, phone);
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = root->right;
        while (temp->left != NULL)
            temp = temp->left;
        strcpy(root->phone, temp->phone);
        strcpy(root->name, temp->name);
        strcpy(root->email, temp->email);
        root->points = temp->points;
        root->right = deleteNode(root->right, temp->phone);
    }
    return root;
}

void displayHeader() {
    printf("---------------------------------------------------------------------\n");
    printf("| Name            | Phone Number   |Email              | Points     | \n");
    printf("---------------------------------------------------------------------\n");
}

void inOrder(Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("| %-15s | %-14s | %-17s | %-10d |\n", root->name, root->phone, root->email, root->points);
        inOrder(root->right);
    }
}

int validatePhone(char phone[]) {
    if (strlen(phone) < 10 || strlen(phone) > 13) return 0;
    for (int i = 0; i < strlen(phone); i++)
        if (!isdigit(phone[i])) return 0;
    return 1;
}

int validateName(char name[]) {
    if (strlen(name) < 5 || strlen(name) > 25) return 0;
    if (strncmp(name, "Mr. ", 4) != 0 && strncmp(name, "Mrs. ", 5) != 0) return 0;
    return 1;
}

int validateEmail(char email[]) {
    if (strlen(email) < 10 || strlen(email) > 20) return 0;
    int count = 0;
    for (int i = 0; i < strlen(email); i++)
        if (email[i] == '@') count++;
    if (count != 1) return 0;
    if (strstr(email, ".com") == NULL && strstr(email, ".co.id") == NULL) return 0;
    return 1;
}

int isValidDrink(char drink[]) {
    return (strcmp(drink, "Cafe Latte") == 0 || strcmp(drink, "Caramel Macchiato") == 0 ||
            strcmp(drink, "Cappuccino") == 0 || strcmp(drink, "Cafe Mocha") == 0);
}

void orderDrinks(Node* user) {
    char drink[50][50]; 
    int quantity[50];     
    int priceDrink[50];   
    int totalPrice = 0;   
    int pointsEarned = 0; 
    int redeemPoints = 0; 
    int freeDrinks = 0;   
    int orderCount = 0;   
    char orderMore;      

    if (user->points >= 25) {
        char usePoints;
        do {
            printf("Do you want to use your points? [You have %d] [y/n]: ", user->points);
            scanf(" %c", &usePoints);
            usePoints = tolower(usePoints);
        } while (usePoints != 'y' && usePoints != 'n');

        if (usePoints == 'y') {
            do {
                printf("How much [%d left] (multiples of 25, min 25): ", user->points);
                scanf("%d", &redeemPoints);
            } while (redeemPoints < 25 || redeemPoints % 25 != 0 || redeemPoints > user->points);

            freeDrinks = redeemPoints / 25;
            user->points -= redeemPoints;
            printf("\nYou have %d free drink(s) left.\n", freeDrinks);

            for (int i = 0; i < freeDrinks; i++) {
                do {
                    printf("Input free drink [Cafe Latte | Caramel Macchiato | Cappuccino | Cafe Mocha]: ");
                    scanf(" %[^\n]", drink[i]);
                } while (!isValidDrink(drink[i]));  
                
                printf("You got a free %s!\n", drink[i]);
            }
        }
    }

    printf("You have %d free drink(s) left.\n\n", freeDrinks);

    do {
        do {
            printf("Input drink [Cafe Latte | Caramel Macchiato | Cappuccino | Cafe Mocha]: ");
            scanf(" %[^\n]", drink[orderCount]);
        } while (!isValidDrink(drink[orderCount]));  
        
        do {
            printf("Input drink quantity [>= 1]: ");
            scanf("%d", &quantity[orderCount]);
        } while (quantity[orderCount] < 1);

        priceDrink[orderCount] = 30000 * quantity[orderCount];
        totalPrice += priceDrink[orderCount];
        orderCount++;

        do {
            printf("Do you want to order more? [y/n]: ");
            scanf(" %c", &orderMore);
            orderMore = tolower(orderMore);
        } while (orderMore != 'y' && orderMore != 'n');

    } while (orderMore == 'y');

    pointsEarned = (totalPrice / 50000) * 3;
    user->points += pointsEarned;

    printf("\nOrder Summary:\n");
    for (int i = 0; i < orderCount; i++) {
        printf("%s - %dx = %d\n", drink[i], quantity[i], priceDrink[i]);
    }
    printf("Total: %d\n", totalPrice);
    printf("Points Obtained: %d\n", pointsEarned);
}

void processOrder(Node** root) {
    char phone[14], name[26], email[21];

    do {
        printf("Input phone number[10-13][numeric]: ");
        scanf("%s", phone);
    } while (!validatePhone(phone));
    
    Node* found = search(*root, phone);

    if (found) {
        printf("User found! Points: %d\n", found->points);
    } else {
        do {
            printf("Input name[5-25][Mr. | Mrs. ]: ");
            scanf(" %[^\n]", name);
        } while (!validateName(name));
        
        do {
            printf("Input email[10-20][email format]: ");
            scanf("%s", email);
        } while (!validateEmail(email));
        
        *root = insert(*root, phone, name, email, 10);  
        printf("Insert success!\n\n");

        found = search(*root, phone); 
    }

    orderDrinks(found);
}

void viewAllCustomer(Node* root) {
    if (root == NULL) {
        printf("There is no data!\n");
    } else {
        displayHeader();
        inOrder(root);
        printf("---------------------------------------------------------------------\n");
    }
}

void removeCustomer(Node** root) {
    char phone[14];
    if (*root == NULL) {
        printf("There is no data!\n");
    } else {
        printf("Enter phone number to remove: ");
        scanf("%s", phone);
        if (search(*root, phone) == NULL) {
            printf("Data invalid!\n");
        } else {
            *root = deleteNode(*root, phone);
            printf("Customer removed successfully!\n");
        }
    }
}

void clearscreen() {
        system("cls");
}

void displayMenu() {
    printf("BlueBucks\n");
    printf("=============\n");
    printf("1. Process Order\n");
    printf("2. View All Customer\n");
    printf("3. Remove Customer\n");
    printf("4. Exit\n");
    printf(">> ");
}

int main() {
    Node* root = NULL;
    int choice;

    while (1) {
        clearscreen();
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                clearscreen();
                processOrder(&root);
                break;
            case 2:
                clearscreen();
                viewAllCustomer(root);
                break;
            case 3:
                clearscreen();
                removeCustomer(&root);
                break;
            case 4:
                clearscreen();
                printf("Exiting program...\n");
                exit(0);
            default:
                clearscreen();
                printf("Invalid choice!\n");
        }
        printf("\nPress Enter to continue...");
        while (getchar() != '\n');
        getchar();
    }
    return 0;
}
