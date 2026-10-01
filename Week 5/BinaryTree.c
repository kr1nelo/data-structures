#include <stdio.h>
#include <stdlib.h>

struct node {
    int key;
    struct node *left, *right;
};

struct node *newNode(int item) {
    struct node *temp = (struct node*)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

void inOrder(struct node *root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d -> ", root->key);
        inOrder(root->right);
    }
}

struct node *insert(struct node *node, int key) {
    if (node == NULL) {
        return newNode(key);
    }
    if (key < node->key) {
        node->left = insert(node->left, key);
    } else {
        node->right = insert(node->right, key);
    }
    return node;
}

struct node *minValueNode(struct node *node) {
    struct node *curr = node;
    while (curr && curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}

struct node *deleteNode(struct node *root, int key) {
    if (root == NULL) {
        return root;
    }
    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == NULL) {
            struct node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct node *temp = root->left;
            free(root);
            return temp;
        }
        struct node *temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

void menu() {
    printf("\n==============Binary Search Tree (BTS)==============\n");
    printf("1. Insert a node\n");
    printf("2. Delete a node\n");
    printf("3. Display tree\n");
    printf("4. Exit\n");
    printf(">> ");
}

void clearScreen() {
    system("cls"); // Gunakan system("clear") jika di Linux/Mac
}

int main() {
    struct node *root = NULL;
    int choice;
    int value;

    do {
        clearScreen();
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                root = insert(root, value); // Menyimpan node yang dimasukkan
                printf("Node %d inserted successfully\n", value);
                break;
            case 2:
                printf("Enter value to delete: ");
                scanf("%d", &value);
                root = deleteNode(root, value);
                printf("Node %d deleted successfully\n", value);
                break;
            case 3:
                if (root == NULL) {
                    printf("Tree is empty.\n");
                } else {
                    printf("In-order traversal:\n");
                    inOrder(root);
                    printf("End\n");
                }
                break;
            case 4:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
        printf("Press enter to continue\n");
        getchar();
        getchar();
    } while (choice != 4);

    return 0;
}
