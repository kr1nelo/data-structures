#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DESC 256
#define MAX_WORD 50

typedef struct TrieNode
{
    struct TrieNode* children[26];
    int isEndOfWord;
    char description[MAX_DESC];
} TrieNode;

TrieNode* createNode()
{
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEndOfWord = 0;
    for(int i = 0; i < 26; ++i)
        node->children[i] = NULL;
    node->description[0] = '\0';
    return node;
}

int isValidWord(const char* word)
{
    if(strlen(word) < 2) return 0;
    for(int i = 0; word[i]; ++i)
	{
        if(isspace(word[i])) return 0;
    }
    return 1;
}

int isValidDesc(const char* desc) 
{
    int space = 0;
    for(int i = 0; desc[i]; ++i)
	{
        if(isspace(desc[i])) space++;
    }
    return space >= 1;
}

void insertWord(TrieNode* root, const char* word, const char* desc)
{
    TrieNode* curr = root;
    for(int i = 0; word[i]; ++i)
	{
        int index = tolower(word[i]) - 'a';
        if(!curr->children[index])
            curr->children[index] = createNode();
        curr = curr->children[index];
    }
    curr->isEndOfWord = 1;

    strncpy(curr->description, desc, MAX_DESC - 1);
    curr->description[MAX_DESC - 1] = '\0';
}

int searchWord(TrieNode* root, const char* word, char* outDesc)
{
    TrieNode* curr = root;
    for(int i = 0; word[i]; ++i)
	{
        int index = tolower(word[i]) - 'a';
        if(!curr->children[index])
		return 0;
        curr = curr->children[index];
    }
    
    if(curr->isEndOfWord)
	{
        strcpy(outDesc, curr->description);
        return 1;
    }
    return 0;
}

void viewAllUtil(TrieNode* node, char* buffer, int depth, int* counter)
{
    if(node->isEndOfWord)
	{
        buffer[depth] = '\0';
        printf("%d. %s\n", (*counter)++, buffer);
    }
    for(int i = 0; i < 26; ++i)
	{
        if(node->children[i])
		{
            buffer[depth] = i + 'a';
            viewAllUtil(node->children[i], buffer, depth + 1, counter);
        }
    }
}

void viewAllWords(TrieNode* root)
{
    char buffer[MAX_WORD];
    int found = 0;

    for(int i = 0; i < 26; ++i)
	{
        if(root->children[i])
		{
            found = 1;
            break;
        }
    }

    if(!found)
	{
        printf("There is no slang word yet in the dictionary.\n");
    }else{
        printf("List of all slang words in the dictionary:\n");
        int counter = 1;
        viewAllUtil(root, buffer, 0, &counter);
    }

    printf("Press enter to continue...");
    getchar();
}

void searchPrefixUtil(TrieNode* node, char* buffer, int depth, int* counter)
{
    if(node->isEndOfWord)
	{
        buffer[depth] = '\0';
        printf("%d. %s\n", (*counter)++, buffer);
    }
    for(int i = 0; i < 26; ++i)
	{
        if(node->children[i])
		{
            buffer[depth] = i + 'a';
            searchPrefixUtil(node->children[i], buffer, depth + 1, counter);
        }
    }
}

void searchPrefix(TrieNode* root)
{
    char prefix[MAX_WORD];
    char temp[MAX_WORD];

    printf("Input a prefix to be searched: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    strcpy(prefix, temp);

    TrieNode* curr = root;
    for(int i = 0; prefix[i]; ++i)
	{
        int index = tolower(prefix[i]) - 'a';
        if(!curr->children[index])
		{
            printf("There is no prefix \"%s\" in the dictionary.\n", prefix);
            printf("Press enter to continue...");
            getchar();
            return;
        }
        curr = curr->children[index];
    }

    printf("Words starts with \"%s\":\n", prefix);
    char buffer[MAX_WORD];
    strcpy(buffer, prefix);
    int counter = 1;
    searchPrefixUtil(curr, buffer, strlen(prefix), &counter);

    printf("Press enter to continue...");
    getchar();
}

void addOrUpdateSlang(TrieNode* root)
{
    char word[MAX_WORD];
    char desc[MAX_DESC];
    char existDesc[MAX_DESC];
    char temp[MAX_DESC];

    do
	{
        printf("Input a new slang word [Must be more than 1 character and contains no space]: ");
        if(!fgets(temp, sizeof(temp), stdin))
		{
            temp[0] = '\0';
        }
        temp[strcspn(temp, "\n")] = '\0';
    }while(!isValidWord(temp));
    strcpy(word, temp);

    do
	{
        printf("Input a new slang word description [Must be more than 2 words]: ");
        if(!fgets(desc, sizeof(desc), stdin)) 
		{
            desc[0] = '\0';
        }
        desc[strcspn(desc, "\n")] = '\0';
    }while(!isValidDesc(desc));

    if(searchWord(root, word, existDesc))
	{
        insertWord(root, word, desc);
        printf("\nSuccessfully updated a slang word.\n");
    }else{
        insertWord(root, word, desc);
        printf("\nSuccessfully released new slang word.\n");
    }

    printf("Press enter to continue...");
    getchar();
}

void searchSlang(TrieNode* root)
{
    char word[MAX_WORD], desc[MAX_DESC];
    char temp[MAX_WORD];

    do
	{
        printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
    }while(!isValidWord(temp));
    strcpy(word, temp);

    if(searchWord(root, word, desc))
	{
        printf("\nSlang word  : %s\nDescription : %s\n", word, desc);
    }else
	{
        printf("\nThere is no word \"%s\" in the dictionary.\n", word);
    }

    printf("Press enter to continue...");
    getchar();
}

void menu(TrieNode* root)
{
    char input[10];
    int choice;
    int valid;

    do
	{
        do
		{
            system("cls");
            printf("Boogle Dictionary\n");
            printf("1. Release a new slang word\n");
            printf("2. Search a slang word\n");
            printf("3. View words by prefix\n");
            printf("4. View all slang words\n");
            printf("5. Exit\n");
            printf("Choose [1-5]: ");

            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            valid = sscanf(input, "%d", &choice);

            if(valid != 1 || choice < 1 || choice > 5)
			{
                printf("Invalid input. Press enter to try again...");
                system("cls");
            }

        }while(valid != 1 || choice < 1 || choice > 5);

		system("cls");
        switch(choice)
		{
            case 1:
                addOrUpdateSlang(root);
                break;
            case 2:
                searchSlang(root);
                break;
            case 3:
                searchPrefix(root);
                break;
            case 4:
                viewAllWords(root);
                break;
            case 5:
                printf("Thank you... Have a nice day :)\n");
                break;
        }

    }while(choice != 5);
}

int main(){
    TrieNode* root = createNode();
    menu(root);
    return 0;
}
