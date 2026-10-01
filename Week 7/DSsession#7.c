#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct AVLNode
{
    char gameTitle[30];
    char gameGenre[20];
    int gameStock;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
};

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int height(struct AVLNode* node)
{
    return node ? node->height : 0;
}

int getBalance(struct AVLNode* node)
{
    return node ? height(node->left) - height(node->right) : 0;
}

struct AVLNode* rightRotate(struct AVLNode* y)
{
    struct AVLNode* x = y->left;
    struct AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

struct AVLNode* leftRotate(struct AVLNode* x)
{
    struct AVLNode* y = x->right;
    struct AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

struct AVLNode* createNode(const char* title, const char* genre, int stock)
{
    struct AVLNode* newNode = (struct AVLNode*)malloc(sizeof(struct AVLNode));
    strcpy(newNode->gameTitle, title);
    strcpy(newNode->gameGenre, genre);
    newNode->gameStock = stock;
    newNode->height = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct AVLNode* insert(struct AVLNode* node, const char* title, const char* genre, int stock)
{
    if(!node) return createNode(title, genre, stock);
    if(strcmp(title, node->gameTitle) < 0) node->left = insert(node->left, title, genre, stock);
    else if(strcmp(title, node->gameTitle) > 0) node->right = insert(node->right, title, genre, stock);
    else return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if(balance > 1 && strcmp(title, node->left->gameTitle) < 0)
        return rightRotate(node);
    if(balance < -1 && strcmp(title, node->right->gameTitle) > 0)
        return leftRotate(node);
    if(balance > 1 && strcmp(title, node->left->gameTitle) > 0)
	{
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if(balance < -1 && strcmp(title, node->right->gameTitle) < 0)
	{
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

struct AVLNode* search(struct AVLNode* root, const char* title)
{
    if(!root || strcmp(title, root->gameTitle) == 0) return root;
    if(strcmp(title, root->gameTitle) < 0) return search(root->left, title);
    else return search(root->right, title);
}

struct AVLNode* insertGame(struct AVLNode* root)
{
    char title[30];
	char genre[20];
    int stock;

    getchar();
	do
	{
	    printf("Input game title [5-25][unique]: ");
	    fgets(title, sizeof(title), stdin);
	    title[strcspn(title, "\n")] = 0;
	} while(strlen(title) < 5 || strlen(title) > 25 || search(root, title));

    do
	{
	    printf("Input game genre [Action|RPG|Adventure|Card Game]: ");
	    scanf("%s", genre);
	} while(!(strcmp(genre, "Action") == 0 || strcmp(genre, "RPG") == 0 || strcmp(genre, "Adventure") == 0 || strcmp(genre, "Card Game") == 0));
	

    do
	{
	    printf("Input Game Stock [>=1]: ");
	    scanf("%d", &stock);
	} while(stock < 1);

    root = insert(root, title, genre, stock);
    printf("Insert success !\n");

    return root;
}

void inOrder(struct AVLNode* root)
{
    if(root)
	{
        inOrder(root->left);
        printf("| %-25s | %-10s | %-10d |\n", root->gameTitle, root->gameGenre, root->gameStock);
        puts("-------------------------------------------------------");
        inOrder(root->right);
    }
}

struct AVLNode* minValueNode(struct AVLNode* node)
{
    struct AVLNode* current = node;
    while(current && current->left)
        current = current->left;
    return current;
}

struct AVLNode* deleteNode(struct AVLNode* root, const char* title)
{
    if(!root) return root;

    if(strcmp(title, root->gameTitle) < 0)
        root->left = deleteNode(root->left, title);
    else if(strcmp(title, root->gameTitle) > 0)
        root->right = deleteNode(root->right, title);
    else
	{
        if(!root->left || !root->right)
		{
            struct AVLNode* temp = root->left ? root->left : root->right;
            if(!temp)
			{
                temp = root;
                root = NULL;
            } else{
                *root = *temp;
            }
            free(temp);
        } else{
            struct AVLNode* temp = minValueNode(root->right);
            strcpy(root->gameTitle, temp->gameTitle);
            strcpy(root->gameGenre, temp->gameGenre);
            root->gameStock = temp->gameStock;
            root->right = deleteNode(root->right, temp->gameTitle);
        }
    }

    if(!root) return root;
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if(balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    if(balance > 1 && getBalance(root->left) < 0)
	{
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if(balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    if(balance < -1 && getBalance(root->right) > 0)
	{
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

struct AVLNode* updateStock(struct AVLNode* root, const char* title)
{
    struct AVLNode* node = search(root, title);
    if(!node)
	{
        printf("Data not found !\n");
        return root;
    } else {
    	printf("Current stock: %d\n\n", node->gameStock);
	}

    char type[10];
    int qty;

    printf("Input update type [add/remove][case sensitive]: ");
    scanf("%s", type);

    if(strcasecmp(type, "add") == 0)
	{
    	do
		{
    		printf("Input stock to add [>= 1]: ");
        	scanf("%d", &qty);
		} while(qty < 1);
        
        node->gameStock += qty;
        printf("Data updated successfully !\n");
    } else if(strcasecmp(type, "remove") == 0)
	{
        do
		{
		    printf("Input stock to remove [1-%d]: ", node->gameStock);
		    scanf("%d", &qty);
		} while(qty < 1 || qty > node->gameStock);
		
        node->gameStock -= qty;
        printf("Data updated successfully !\n");
        
        if(node->gameStock == 0)
		{
            printf("%s is removed from the warehouse !\n", node->gameTitle);
            root = deleteNode(root, title);
        }
    } else{
        printf("Invalid update type!\n");
    }

    return root;
}

void printMenu()
{
    puts("Bluejack GShop");
    puts("==============");
    puts("1. Insert Game");
    puts("2. View Game");
    puts("3. Update Stock");
    puts("4. Exit");
    printf(">> ");
}

int main()
{
    int choice = 0;
    struct AVLNode* root = NULL;
    char title[30];

    while(choice != 4)
	{
        system("cls");
        printMenu();
        scanf("%d", &choice);
        printf("\n");

        switch(choice)
		{
            case 1:
            	system("cls");
                root = insertGame(root);
                break;
                
            case 2:
            	system("cls");
                if(!root)
				{
                    printf("Warehouse is empty !\n");
                } else {
                	puts("-------------------------------------------------------");
                    printf("| %-25s | %-10s | %s | \n", "Game Title", "Game Genre", "Game Stock" );
                    puts("-------------------------------------------------------");
                    inOrder(root);
                }
                break;
                
            case 3:
            	system("cls");
                getchar();
                printf("Input game title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                
                root = updateStock(root, title);
                break;
                
            case 4:
                printf("Thank you for using this program...\n");
                break;
                
            default:
                printf("Please input number between 1 and 4\n");
        }

		printf("\n");
        printf("Press enter to continue...\n");
        getch();
    }

    return 0;
}
