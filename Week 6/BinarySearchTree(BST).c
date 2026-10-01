#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

struct node {
    int data;
    struct node *left, *right;
};

struct node* root = NULL;

struct node* createnode(int value) 
{
    struct node* newnode = (struct node*)malloc(sizeof(struct node));
    newnode->data = value;
    newnode->left = newnode->right = NULL;
    return newnode;
}

struct node* insert(struct node* root, int value) 
{
    if (root == NULL) 
	{
        return createnode(value);
    }
    
    if (value < root->data) 
	{
        root->left = insert(root->left, value);
    } 
	else if (value > root->data) 
	{
        root->right = insert(root->right, value);
    }
    return root;
}

void inorder(struct node* root) 
{
    if (root != NULL) 
	{
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

void preorder(struct node* root) 
{
    if (root != NULL) 
	{
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(struct node* root) 
{
    if (root != NULL) 
	{
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

// Fungsi untuk menemukan node dengan nilai terkecil dari tree
struct node* findMin(struct node* root) 
{
    while (root->left != NULL)
	{
        root = root->left;
    }
    return root;
}

struct node* deletenode(struct node* root, int value) 
{
    if (root == NULL)
	{
        return root;
    }
    
    if (value < root->data) 
	{
        root->left = deletenode(root->left, value);
    } 
	else if (value > root->data) 
	{
        root->right = deletenode(root->right, value);
    } 
	else 
	{
		// Case 1: nodenya memiliki 1 child atau tidak memiliki child sama sekali
        if (root->left == NULL) 
		{
            struct node* temp = root->right;
            free(root);
            return temp;
        }
		else if (root->right == NULL) 
		{
            struct node* temp = root->left;
            free(root);
            return temp;
        }
        
        // Case 2: node dengan 2 child
        struct node* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deletenode(root->right, temp->data);
    }
    
    return root;
}

void printMenu()
{
	printf("Binary Tree Menu:\n");
	printf("=====================\n");
    printf("1. Insert Data\n");
    printf("2. View Data (In-order, Pre-order, Post-order)\n");
    printf("3. Delete Data\n");
    printf("4. Exit\n");
    printf("=====================\n");
    printf("Enter your choice: ");
}

void insertMenu()
{
	int value;
	printf("Enter value to insert: ");
    scanf("%d", &value);
    root = insert(root, value);
}

void viewMenu()
{
	printf("\nIn-order Traversal: ");
    inorder(root);
    printf("\nPre-order Traversal: ");
    preorder(root);
    printf("\nPost-order Traversal: ");
    postorder(root);
    printf("\n");
}

void deleteMenu()
{
	int value;
	
	printf("Enter value to delete: ");
    scanf("%d", &value);
    root = deletenode(root, value);
}

int main() 
{
    int choice;
    
    while (choice != 4) 
	{
    	system("cls");
        printMenu();
        scanf("%d", &choice);
        
        switch (choice) 
		{
            case 1:
                insertMenu();
                break;
            case 2:
                viewMenu();
                break;
            case 3:
                deleteMenu();
                break;
            case 4:
                printf("Thank you for using this program...\n");
                break;
            default:
                printf("Please input number between 1 and 4\n");
        }
        
        printf("Press any key to continue...\n");
        getch();
    }
    
    return 0;
}
