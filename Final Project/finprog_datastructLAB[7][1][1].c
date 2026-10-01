#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 100
#define FILE_NAME "accounts.txt"

typedef struct Account{
    char username[50];
    char password[50];
    struct Account* next;
}Account;

Account* hashTable[SIZE];

int hashFunction(const char* str){
    int sum = 0;
    while(*str){
        sum += *str;
        str++;
    }
    return sum % SIZE;
}

Account* searchAccount(const char* username){
    int index = hashFunction(username);
    Account* temp = hashTable[index];
    while(temp){
        if(strcmp(temp->username, username) == 0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void createAccount(char* username, char* password){
    Account* newAcc = (Account*)malloc(sizeof(Account));
    strcpy(newAcc->username, username);
    strcpy(newAcc->password, password);
    newAcc->next = NULL;

    int index = hashFunction(username);
    if(hashTable[index] == NULL){
        hashTable[index] = newAcc;
    }else{
        Account* temp = hashTable[index];
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newAcc;
    }
}

void deleteAccount(char* username){
    int index = hashFunction(username);
    Account* temp = hashTable[index];
    Account* prev = NULL;

    while(temp){
        if(strcmp(temp->username, username) == 0){
            if(prev == NULL){
                hashTable[index] = temp->next;
            }else{
                prev->next = temp->next;
            }
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Akun tidak ditemukan\n");
}

void updateAccount(char* username, char* newPassword){
    Account* acc = searchAccount(username);
    if(acc){
        strcpy(acc->password, newPassword);
    }
}

int checkPassword(char* username, char* password){
    Account* acc = searchAccount(username);
    return acc && strcmp(acc->password, password) == 0;
}

int isValidPassword(const char *password){
    int hasLetter = 0, hasDigit = 0;
    int len = strlen(password);
    if(len < 5 || len > 20) return 0;
    for(int i = 0; i < len; i++){
        if(isalpha(password[i])) hasLetter = 1;
        if(isdigit(password[i])) hasDigit = 1;
    }
    return hasLetter && hasDigit;
}

void saveAccountsToFile(){
    FILE *fp = fopen(FILE_NAME, "w");
    if(fp == NULL){
        printf("Gagal membuka file untuk menyimpan akun.\n");
        return;
    }

    for(int i = 0; i < SIZE; i++){
        Account *temp = hashTable[i];
        while(temp){
            fprintf(fp, "%s %s\n", temp->username, temp->password);
            temp = temp->next;
        }
    }

    fclose(fp);
}

void loadAccountsFromFile(){
    FILE *fp = fopen(FILE_NAME, "r");
    if(fp == NULL) return;

    char username[50], password[50];
    while(fscanf(fp, "%s %s", username, password) == 2){
        createAccount(username, password);
    }

    fclose(fp);
}

void buatAkun(){
    char username[50], password[50];
    system("cls");
    printf("---MAKE ACCOUNT---\n");
    printf("Ketik '0' untuk membatalkan pembuatan akun\n");

    while(1){
        printf("Buat Email: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        if(strcmp(username, "0") == 0){
            system("cls");
            printf("Pembuatan akun dibatalkan\n");
            printf("Tekan enter untuk kembali");
            getchar();
            system("cls");
            return;
        }

        char *at_sign = strchr(username, '@');
        char *dot = strrchr(username, '.');

        if(at_sign == NULL || dot == NULL || at_sign > dot){
            printf("Format email tidak valid. Contoh: user@example.com\n");
            continue;
        }

        int prefix_len = at_sign - username;
        if(prefix_len < 5 || prefix_len > 20){
            printf("Email harus memiliki 5-20 karakter sebelum '@'\n");
            continue;
        }

        Account* existingAcc = searchAccount(username);
        if(existingAcc != NULL){
            printf("Email sudah digunakan, silakan masukkan Email lain\n");
            continue;
        }

        break;
    }

    while(1){
        printf("Buat password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        if(strcmp(password, "0") == 0){
            system("cls");
            printf("Pembuatan akun dibatalkan\n");
            printf("Tekan enter untuk kembali");
            getchar();
            system("cls");
            return;
        }

        if(!isValidPassword(password)){
            printf("Password harus 5-20 karakter dan mengandung kombinasi huruf dan angka\n");
            continue;
        }

        break;
    }

    createAccount(username, password);
    saveAccountsToFile();

    system("cls");
    printf("Akun berhasil dibuat\n");
    printf("Tekan enter untuk kembali");
    getchar();
}

void tampilkanAkun(){
    system("cls");
    int isEmpty = 1;
    for(int i = 0; i < SIZE; i++){
        if(hashTable[i] != NULL){
            isEmpty = 0;
            break;
        }
    }

    if(isEmpty){
        printf("---VIEW LIST ACCOUNT---\n");
        printf("Belum ada akun yang dibuat\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        return;
    }

    system("cls");
    printf("---------------VIEW LIST ACCOUNT---------------\n");
    printf("+----------------------+----------------------+\n");
    printf("|      Email           |       Password       |\n");
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
    system("cls");
}

void cariAkun(){
    system("cls");
    int isEmpty = 1;
    for(int i = 0; i < SIZE; i++){
        if(hashTable[i] != NULL){
            isEmpty = 0;
            break;
        }
    }

    if(isEmpty){
        printf("---SEARCH ACCOUNT---\n");
        printf("Belum ada akun yang dibuat\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        return;
    }

    char choice[10];
    do{
        system("cls");
        printf("---SEARCH ACCOUNT---\n");
        printf("1.Cari akun\n");
        printf("0.Batalkan dan kembali ke menu\n");
        printf("Pilihan [0/1]: ");

        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;

        if(strcmp(choice, "0") == 0){
            system("cls");
            return;
        }else if(strcmp(choice, "1") == 0){
            char username[50];
            printf("Masukkan Email yang dicari (atau 0 untuk batal): ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = 0;

            if(strcmp(username, "0") == 0) continue;

            system("cls");
            Account* found = searchAccount(username);
            if(found){
                printf("----------------ACCOUNT FOUND------------------\n");
                printf("+----------------------+----------------------+\n");
                printf("|      Email           |       Password       |\n");
                printf("+----------------------+----------------------+\n");
                printf("| %-20s | %-20s |\n", found->username, found->password);
                printf("+----------------------+----------------------+\n");
            }else{
                printf("Akun tidak ditemukan\n");
            }
            printf("Tekan enter untuk melanjutkan...");
            getchar();
        }else{
            printf("Input tidak valid! Harap masukkan 0 atau 1\n");
            printf("Tekan enter untuk mencoba lagi...");
            getchar();
        }
    }while(1);
}

void ubahPassword(){
    system("cls");
    int isEmpty = 1;
    for(int i = 0; i < SIZE; i++){
        if(hashTable[i] != NULL){
            isEmpty = 0;
            break;
        }
    }

    if(isEmpty){
        printf("---CHANGE PASSWORD---\n");
        printf("Belum ada akun yang dibuat\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        return;
    }

    char choice[10];
    do{
        system("cls");
        printf("---CHANGE PASSWORD---\n");
        printf("1. Ubah password\n");
        printf("0. Batalkan dan kembali ke menu\n");
        printf("Pilihan [0/1]: ");

        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;

        if(strcmp(choice, "0") == 0){
            system("cls");
            return;
        }else if(strcmp(choice, "1") == 0){
            char username[50], oldPassword[50], newPassword[50];
            printf("Masukkan Email (atau 0 untuk batal): ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = 0;
            if(strcmp(username, "0") == 0) continue;

            printf("Masukkan password lama (atau 0 untuk batal): ");
            fgets(oldPassword, sizeof(oldPassword), stdin);
            oldPassword[strcspn(oldPassword, "\n")] = 0;
            if(strcmp(oldPassword, "0") == 0) continue;

            if(!checkPassword(username, oldPassword)){
                printf("Email/Password salah\n");
                printf("Tekan enter untuk kembali");
                getchar();
                continue;
            }

            do{
                printf("Masukkan password baru (atau 0 untuk batal): ");
                fgets(newPassword, sizeof(newPassword), stdin);
                newPassword[strcspn(newPassword, "\n")] = 0;

                if(strcmp(newPassword, "0") == 0) break;
                if(strcmp(newPassword, oldPassword) == 0){
                    printf("Password baru tidak boleh sama dengan password lama\n");
                    continue;
                }
                if(!isValidPassword(newPassword)){
                    printf("Password harus 5-20 karakter dan mengandung kombinasi huruf dan angka\n");
                    continue;
                }
                break;
            }while(1);

            if(strcmp(newPassword, "0") != 0){
                updateAccount(username, newPassword);
                saveAccountsToFile();
                printf("Password berhasil diubah.\n");
            }

            printf("Tekan enter untuk melanjutkan...");
            getchar();
        }else{
            printf("Input tidak valid!\n");
            printf("Tekan enter untuk mencoba lagi...");
            getchar();
        }
    }while(1);
}

void hapusAkun(){
    system("cls");
    int isEmpty = 1;
    for(int i = 0; i < SIZE; i++){
        if(hashTable[i] != NULL){
            isEmpty = 0;
            break;
        }
    }

    if(isEmpty){
        printf("---DELETE ACCOUNT---\n");
        printf("Belum ada akun yang dibuat\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        return;
    }

    char choice[10];
    do{
        system("cls");
        printf("---DELETE ACCOUNT---\n");
        printf("1.Hapus akun\n");
        printf("0.Batalkan dan kembali ke menu\n");
        printf("Pilihan [0/1]: ");

        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;

        if(strcmp(choice, "0") == 0){
            system("cls");
            return;
        }else if(strcmp(choice, "1") == 0){
            char username[50], enteredPassword[50];

            printf("Masukkan Email (atau 0 untuk batal): ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = 0;
            if(strcmp(username, "0") == 0) continue;

            Account* target = searchAccount(username);
            if(!target){
                printf("Email tidak ditemukan\n");
                printf("Tekan enter untuk melanjutkan...");
                getchar();
                continue;
            }

            printf("Masukkan password untuk konfirmasi (atau 0 untuk batal): ");
            fgets(enteredPassword, sizeof(enteredPassword), stdin);
            enteredPassword[strcspn(enteredPassword, "\n")] = 0;
            if(strcmp(enteredPassword, "0") == 0) continue;

            if(strcmp(target->password, enteredPassword) != 0){
                printf("Password salah\n");
                printf("Tekan enter untuk melanjutkan...");
                getchar();
                continue;
            }

            deleteAccount(username);
            saveAccountsToFile();
            printf("Akun berhasil dihapus\n");
            printf("Tekan enter untuk melanjutkan...");
            getchar();
        }else{
            printf("Input tidak valid! Harap masukkan 0 atau 1\n");
            printf("Tekan enter untuk mencoba lagi...");
            getchar();
        }
    }while(1);
}

int main(){
    int choice;
    char input[100]; 

    loadAccountsFromFile();

    while(1){
        system("cls");
        printf("PASSWORD MANAGER\n");
        printf("1.Buat akun\n");
        printf("2.Tampilkan semua akun\n");
        printf("3.Cari akun\n");
        printf("4.Ubah password\n");
        printf("5.Hapus akun\n");
        printf("6.Keluar\n");

        while(1){
            printf(">> ");
            fgets(input, sizeof(input), stdin);
            if(sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= 6){
                break;
            }
            printf("Input tidak valid! Masukkan angka 1-6\n");
        }

        switch(choice){
            case 1: 
				buatAkun(); 
				break;
            case 2: 
				tampilkanAkun(); 
				break;
            case 3: 
				cariAkun(); 
				break;
            case 4: 
				ubahPassword(); 
				break;
            case 5: 
				hapusAkun(); 
				break;
            case 6:
                system("cls");
                printf("Terima kasih telah menggunakan Password Manager\n");
                return 0;
        }
    }
}
