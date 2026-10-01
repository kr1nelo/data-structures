#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 100

typedef struct Account{
    char username[50];
    char password[50];
    struct Account* next;
}Account;

Account* hashTable[SIZE];

int hashFunction(char* key) { //untuk hash berdasrkan jumlah karakter dan ascii
    int hash = 0;
    while(*key){
        hash = (hash + *key) % SIZE;
        key++;
    }
    return hash;
}

void createAccount(char* username, char* password){
    while(1){
        int len = strlen(username);
        if(len < 5 || len > 20){
            printf("Username harus memiliki 5 hingga 20 karakter.\n");
            printf("Masukkan username kembali: ");
            fgets(username, 100, stdin);
            username[strcspn(username, "\n")] = 0;
            continue;
        }
        
        if(strstr(username, "@gmail.com") == NULL){
            printf("Username harus mengandung '@gmail.com'.\n");
            printf("Masukkan username kembali: ");
            fgets(username, 100, stdin);
            username[strcspn(username, "\n")] = 0;
        }
        break;
    }

    int index = hashFunction(username);
    Account* newAcc = (Account*)malloc(sizeof(Account));
    strcpy(newAcc->username, username);
    strcpy(newAcc->password, password);
    newAcc->next = hashTable[index];
    hashTable[index] = newAcc;
    
    printf("Akun berhasil dibuat.\n");
    printf("Tekan enter untuk kembali");
    getchar();
}

void readAccounts(){ //untuk tampilkan semua akun
    printf("---LIST ACCOUNT---\n");
    for(int i = 0; i < SIZE; i++){
        Account* temp = hashTable[i];
        while(temp){
            printf("Username: %s\n", temp->username);
            printf("Password: %s\n", temp->password);
            temp = temp->next;
            getchar();
        }
    }
}

Account* searchAccount(char* username){ //untuk mencari akun
    int index = hashFunction(username);
    Account* temp = hashTable[index];
    while(temp){
        if(strcmp(temp->username, username) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void updateAccount(char* username, char* newPassword){ //untuk ubah password
    Account* acc = searchAccount(username);
    if(acc){
        strcpy(acc->password, newPassword);
        printf("Password berhasil diperbarui.\n");
    }else{
        printf("Akun tidak ditemukan.\n");
    }
}

void deleteAccount(char* username){ //untuk hapus akun
    int index = hashFunction(username);
    Account* temp = hashTable[index];
    Account* prev = NULL;
    while(temp){
        if(strcmp(temp->username, username) == 0){
            if(prev)
                prev->next = temp->next;
            else
                hashTable[index] = temp->next;
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Akun tidak ditemukan.\n");
}

int main() 
{
    int choice;
    char username[50];
	char password[50];

    while(1){
        printf("PASSWORD MANAGER\n");
        printf("1.Buat akun\n");
        printf("2.Tampilkan semua akun\n");
        printf("3.Cari akun\n");
        printf("4.Ubah password\n");
        printf("5.Hapus akun\n");
        printf("6.Keluar\n");
        printf(">> ");
        scanf("%d", &choice);
        getchar();

        switch(choice){
            case 1:
    			system("cls");
    			printf("---MAKE ACCOUNT---\n");
    while(1){
        printf("Masukkan username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;
        int len = strlen(username);
        if(len < 5 || len > 20){
            printf("Username harus memiliki 5 hingga 20 karakter.\n");
            continue;
        }
        if(strstr(username, "@gmail.com") == NULL){
            printf("Username harus mengandung '@gmail.com'.\n");
            continue;
        }
        
        // Check if account already exists
        Account* existingAcc = searchAccount(username);
        if(existingAcc != NULL){
            printf("Username sudah digunakan, silakan masukkan username lain.\n");
            continue;
        }
        break;
    }
    
    while(1){
        // Password input and validation
        printf("Masukkan password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;
        int pwd_len = strlen(password);
        if(pwd_len < 5 || pwd_len > 20){
            printf("Password harus memiliki 5 hingga 20 karakter.\n");
            continue;
        }
        break;
    }
    
    createAccount(username, password);
    system("cls");
    break;
                
case 2:{
    system("cls");
    int isEmpty = 1;
    for(int i = 0; i < SIZE; i++){
        if(hashTable[i] != NULL){
            isEmpty = 0;
            break;
        }
    }
    if(isEmpty){
        printf("Belum ada akun yang dibuat.\n");
        printf("Tekan enter untuk kembali");
        getchar();
    }else{
    	printf("---VIEW LIST ACCOUNT---\n");
        printf("+----------------------+----------------------+\n");
        printf("|      Username        |       Password       |\n");
        printf("+----------------------+----------------------+\n");
        for(int i = 0; i < SIZE; i++){
            Account* temp = hashTable[i];
            while(temp){
                printf("| %-20s | %-20s |\n", temp->username, temp->password);
                temp = temp->next;
            }
        }
        printf("+----------------------+----------------------+\n");
        printf("Tekan enter untuk kembali");
        getchar();
    }
    system("cls");
    break;
}
			
            case 3:
    system("cls");
    int isEmpty3 = 1;
    for(int i = 0; i < SIZE; i++){
        if(hashTable[i] != NULL){
            isEmpty3 = 0;
            break;
        }
    }
    if(isEmpty3){
    	printf("---SEARCH ACCOUNT---\n");
        printf("Belum ada akun yang dibuat.\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        break;
    }
    
    printf("---SEARCH ACCOUNT---\n");
    printf("Masukkan username yang dicari: ");
    fgets(username, sizeof(username), stdin);
	username[strcspn(username, "\n")] = 0;
    Account* found = searchAccount(username);
    if(found){
        printf("Ditemukan\n");
        printf("Username: %s\n", found->username);  
        printf("Password: %s\n", found->password);
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
    }else{
        printf("Akun tidak ditemukan\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
    }
    break;
                
            case 4:
            	system("cls");
    int isEmpty4 = 1;
    for(int i = 0; i < SIZE; i++){
        if(hashTable[i] != NULL){
            isEmpty4 = 0;
            break;
        }
    }
    if(isEmpty4){
    	printf("---CHANGE PASSWORD---\n");
        printf("Belum ada akun yang dibuat.\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        break;
    }
    
    printf("---CHANGE PASSWORD---\n");
    printf("Masukkan username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("Masukkan password baru: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;
    updateAccount(username, password);
    printf("Tekan enter untuk kembali");
    getchar();
    system("cls");
    break;
    			
            case 5:
            	system("cls");
    int isEmpty5 = 1;
    for(int i = 0; i < SIZE; i++){
        if(hashTable[i] != NULL){
            isEmpty5 = 0;
            break;
        }
    }
    if(isEmpty5){
        printf("Belum ada akun yang dibuat.\n");
        printf("Tekan enter untuk kembali");
        getchar();
        break;
    }
    
    printf("Masukkan username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    Account* target = searchAccount(username);
    if(target == NULL){
        printf("Username/password salah\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        break;
    }
    
    char enteredPassword[50];
    printf("Masukkan password untuk konfirmasi: ");
    fgets(enteredPassword, sizeof(enteredPassword), stdin);
    enteredPassword[strcspn(enteredPassword, "\n")] = 0;
    if(strcmp(target->password, enteredPassword) != 0){
        printf("Username/password salah\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        break;
    }
    
    deleteAccount(username);
    printf("Akun berhasil dihapus\n");
    printf("Tekan enter untuk kembali");
    getchar();
    system("cls");
    break;
                
            case 6:
            	system("cls");
                printf("Terima kasih telah menggunakan Password Manager.\n");
                return 0;
            default:
                printf("Pilihan tidak valid.\n");
        }
    }
}
