#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

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
            printf("Akun berhasil dihapus\n");
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
    }else{
        printf("Akun tidak ditemukan\n");
    }
}

int checkPassword(char* username, char* password){
    Account* acc = searchAccount(username);
    if(acc && strcmp(acc->password, password) == 0){
        return 1;
    }
    return 0;
}

void buatAkun() {
    char username[50], password[50];
    system("cls");
    printf("---MAKE ACCOUNT---\n");
    printf("Ketik '0' untuk membatalkan pembuatan akun.\n");

    while(1) {
        printf("Masukkan username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        // Cancel option
        if(strcmp(username, "0") == 0) {
            system("cls");
            printf("Pembuatan akun dibatalkan\n");
            printf("Tekan enter untuk kembali");
            getchar();
            system("cls");
            return;
        }

        int len = strlen(username);
        char *gmail_pos = strstr(username, "@gmail.com");

        // Validate username format
        if(gmail_pos == NULL) {
            printf("Username harus mengandung '@gmail.com'\n");
            continue;
        }

        // Calculate length before @gmail.com
        int prefix_len = gmail_pos - username;

        if(prefix_len < 5 || prefix_len > 20) {
            printf("Username harus memiliki 5-20 karakter sebelum '@gmail.com'\n");
            continue;
        }

        // Check if @gmail.com is at the end
        if(strcmp(gmail_pos, "@gmail.com") != 0) {
            printf("'@gmail.com' harus di akhir username\n");
            continue;
        }

        // Check if username already exists
        Account* existingAcc = searchAccount(username);
        if(existingAcc != NULL) {
            printf("Username sudah digunakan, silakan masukkan username lain\n");
            continue;
        }

        break;
    }

    while(1) {
        printf("Masukkan password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        if(strcmp(password, "0") == 0) {
            system("cls");
            printf("Pembuatan akun dibatalkan\n");
            printf("Tekan enter untuk kembali");
            getchar();
            system("cls");
            return;
        }

        int pwd_len = strlen(password);
        if(pwd_len < 5 || pwd_len > 20) {
            printf("Password harus memiliki 5 hingga 20 karakter.\n");
            continue;
        }
        break;
    }

    createAccount(username, password);
    system("cls");
    printf("Akun berhasil dibuat.\n");
    printf("Tekan enter untuk kembali");
    getchar();
}

void tampilkanAkun() {
    system("cls");
    
    // First check if there are any accounts
    int isEmpty = 1;
    for(int i = 0; i < SIZE; i++) {
        if(hashTable[i] != NULL) {
            isEmpty = 0;
            break;
        }
    }
    
    if(isEmpty) {
        printf("---VIEW LIST ACCOUNT---\n");
        printf("Belum ada akun yang dibuat.\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        return;
    }
    
    // If there are accounts, show the menu
    char choice[10];
    int validInput = 0;

    do {
        system("cls");
        printf("---VIEW LIST ACCOUNT---\n");
        printf("1. Tampilkan semua akun\n");
        printf("0. Batalkan dan kembali ke menu\n");
        printf("Pilihan [0/1]: ");
        
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;
        
        if(strcmp(choice, "0") == 0) {
            system("cls");
            return;
        }
        else if(strcmp(choice, "1") == 0) {
            validInput = 1;
        }
        else {
            printf("\nInput tidak valid! Harap masukkan 0 atau 1.\n");
            printf("Tekan enter untuk mencoba lagi...");
            getchar();
        }
    } while(!validInput);

    // Show all accounts
    system("cls");
    printf("---------------VIEW LIST ACCOUNT---------------\n");
    printf("+----------------------+----------------------+\n");
    printf("|      Username        |       Password       |\n");
    printf("+----------------------+----------------------+\n");
    for(int i = 0; i < SIZE; i++) {
        Account* temp = hashTable[i];
        while(temp) {
            printf("| %-20s | %-20s |\n", temp->username, temp->password);
            temp = temp->next;
        }
    }
    printf("+----------------------+----------------------+\n");
    
    printf("Tekan enter untuk kembali");
    getchar();
    system("cls");
}

void cariAkun() {
    system("cls");
    
    // First check if there are any accounts
    int isEmpty = 1;
    for(int i = 0; i < SIZE; i++) {
        if(hashTable[i] != NULL) {
            isEmpty = 0;
            break;
        }
    }
    
    if(isEmpty) {
        printf("---SEARCH ACCOUNT---\n");
        printf("Belum ada akun yang dibuat.\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        return;
    }
    
    // If accounts exist, show the search menu
    char choice[10];
    int validInput;
    
    do {
        system("cls");
        printf("---SEARCH ACCOUNT---\n");
        printf("1. Cari akun\n");
        printf("0. Batalkan dan kembali ke menu\n");
        printf("Pilihan [0/1]: ");
        
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;
        
        if(strcmp(choice, "0") == 0) {
            system("cls");
            return;
        }
        else if(strcmp(choice, "1") == 0) {
            validInput = 1;
        }
        else {
            printf("\nInput tidak valid! Harap masukkan 0 atau 1.\n");
            printf("Tekan enter untuk mencoba lagi...");
            getchar();
            continue;
        }
        
        // Search process
        char username[50];
        printf("\nMasukkan username yang dicari (atau 0 untuk batal): ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;
        
        if(strcmp(username, "0") == 0) {
            continue;
        }
        
        system("cls");
        Account* found = searchAccount(username);
        if(found) {
            printf("----------------ACCOUNT FOUND------------------\n");
            printf("+----------------------+----------------------+\n");
            printf("|      Username        |       Password       |\n");
            printf("+----------------------+----------------------+\n");
            printf("| %-20s | %-20s |\n", found->username, found->password);
            printf("+----------------------+----------------------+\n");
        } else {
            printf("Akun tidak ditemukan\n");
        }
        
        printf("\nTekan enter untuk melanjutkan...");
        getchar();
        
    } while(1);  // Infinite loop, exits through return statement
    
    system("cls");
}

void ubahPassword() {
    system("cls");
    
    // First check if there are any accounts
    int isEmpty = 1;
    for(int i = 0; i < SIZE; i++) {
        if(hashTable[i] != NULL) {
            isEmpty = 0;
            break;
        }
    }
    
    if(isEmpty) {
        printf("---CHANGE PASSWORD---\n");
        printf("Belum ada akun yang dibuat.\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        return;
    }
    
    // If accounts exist, show the password change menu
    char choice[10];
    int validInput;
    
    do {
        system("cls");
        printf("---CHANGE PASSWORD---\n");
        printf("1. Ubah password\n");
        printf("0. Batalkan dan kembali ke menu\n");
        printf("Pilihan [0/1]: ");
        
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;
        
        if(strcmp(choice, "0") == 0) {
            system("cls");
            return;
        }
        else if(strcmp(choice, "1") == 0) {
            validInput = 1;
        }
        else {
            printf("\nInput tidak valid! Harap masukkan 0 atau 1.\n");
            printf("Tekan enter untuk mencoba lagi...");
            getchar();
            continue;
        }
        
        // Password change process
        char username[50], password[50], oldPassword[50];
        
        printf("\nMasukkan username (atau 0 untuk batal): ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;
        
        if(strcmp(username, "0") == 0) {
            continue;
        }

        // Validate username format
        int len = strlen(username);
        if(len < 10 || strcmp(&username[len - 10], "@gmail.com") != 0) {
            printf("Username tidak valid. Harus berakhiran dengan @gmail.com\n");
            printf("Tekan enter untuk kembali");
            getchar();
            continue;
        }

        printf("Masukkan password lama (atau 0 untuk batal): ");
        fgets(oldPassword, sizeof(oldPassword), stdin);
        oldPassword[strcspn(oldPassword, "\n")] = 0;
        
        if(strcmp(oldPassword, "0") == 0) {
            continue;
        }

        if(!checkPassword(username, oldPassword)) {
            printf("Password lama salah.\n");
            printf("Tekan enter untuk kembali");
            getchar();
            continue;
        }

        // Get new password with option to cancel
        do {
            printf("Masukkan password baru (atau 0 untuk batal): ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = 0;
            
            if(strcmp(password, "0") == 0) {
                break;
            }

            if(strcmp(password, oldPassword) == 0) {
                printf("Password baru tidak boleh sama dengan password lama.\n");
            }
        } while(strcmp(password, oldPassword) == 0);

        if(strcmp(password, "0") != 0) {
            updateAccount(username, password);
            printf("\nPassword berhasil diubah.\n");
        }
        
        printf("Tekan enter untuk melanjutkan...");
        getchar();
        
    } while(1);  // Infinite loop, exits through return statement
    
    system("cls");
}

void hapusAkun() {
    system("cls");
    
    // First check if there are any accounts
    int isEmpty = 1;
    for(int i = 0; i < SIZE; i++) {
        if(hashTable[i] != NULL) {
            isEmpty = 0;
            break;
        }
    }
    
    if(isEmpty) {
        printf("---DELETE ACCOUNT---\n");
        printf("Belum ada akun yang dibuat.\n");
        printf("Tekan enter untuk kembali");
        getchar();
        system("cls");
        return;
    }
    
    // If accounts exist, show the delete menu
    char choice[10];
    int validInput;
    
    do {
        system("cls");
        printf("---DELETE ACCOUNT---\n");
        printf("1. Hapus akun\n");
        printf("0. Batalkan dan kembali ke menu\n");
        printf("Pilihan [0/1]: ");
        
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;
        
        if(strcmp(choice, "0") == 0) {
            system("cls");
            return;
        }
        else if(strcmp(choice, "1") == 0) {
            validInput = 1;
        }
        else {
            printf("\nInput tidak valid! Harap masukkan 0 atau 1.\n");
            printf("Tekan enter untuk mencoba lagi...");
            getchar();
            continue;
        }
        
        // Account deletion process
        char username[50], enteredPassword[50];
        
        printf("\nMasukkan username (atau 0 untuk batal): ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;
        
        if(strcmp(username, "0") == 0) {
            continue;
        }

        Account* target = searchAccount(username);
        if(!target) {
            printf("Username tidak ditemukan\n");
            printf("Tekan enter untuk melanjutkan...");
            getchar();
            continue;
        }

        printf("Masukkan password untuk konfirmasi (atau 0 untuk batal): ");
        fgets(enteredPassword, sizeof(enteredPassword), stdin);
        enteredPassword[strcspn(enteredPassword, "\n")] = 0;
        
        if(strcmp(enteredPassword, "0") == 0) {
            continue;
        }

        if(strcmp(target->password, enteredPassword) != 0) {
            printf("Password salah\n");
            printf("Tekan enter untuk melanjutkan...");
            getchar();
            continue;
        }

        deleteAccount(username);
        printf("\nAkun berhasil dihapus.\n");
        printf("Tekan enter untuk melanjutkan...");
        getchar();
        
    } while(1);  // Infinite loop, exits through return statement
    
    system("cls");
}

int main() 
{
    int choice;
    char input[100]; // Buffer for input
    
    while(1){
        system("cls");
        printf("PASSWORD MANAGER\n");
        printf("1. Buat akun\n");
        printf("2. Tampilkan semua akun\n");
        printf("3. Cari akun\n");
        printf("4. Ubah password\n");
        printf("5. Hapus akun\n");
        printf("6. Keluar\n");
        
        // Input validation loop
        while(1) {
            printf(">> ");
            fgets(input, sizeof(input), stdin);
            
            // Check if input is a valid number
            if(sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= 6) {
                break; // Valid input, exit the validation loop
            }
            
            printf("Input tidak valid! Masukkan angka 1-6.\n");
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
                printf("Terima kasih telah menggunakan Password Manager.\n");
                return 0;
        }
    }
}
