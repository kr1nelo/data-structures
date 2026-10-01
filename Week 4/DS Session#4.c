#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_TABLE_SIZE 1007
#define MEDICINE_NAME_MIN 5
#define MEDICINE_NAME_MAX 50
#define MANUFACTURER_NAME_MIN 3
#define MANUFACTURER_NAME_MAX 25
#define REGISTRATION_NUMBER_LENGTH 10
#define BATCH_LOT_MIN 16

typedef struct Medicine{
    char medicineId[21];
    char medicineName[MEDICINE_NAME_MAX + 1];
    char manufacturerName[MANUFACTURER_NAME_MAX + 1];
    char registrationNo[REGISTRATION_NUMBER_LENGTH + 1];
    int batchLotNumber;
    struct Medicine* next;
}Medicine;

Medicine* hashTable[HASH_TABLE_SIZE];
int lastMedicineIdNumber = 0;

void initHashTable(){
    for(int i = 0; i < HASH_TABLE_SIZE; i++){
        hashTable[i] = NULL;
    }
}

int hashFunction(char* medicineId){
    int sum = 0;
    for(int i = 0; medicineId[i] != '\0'; i++){
        sum += medicineId[i];
    }
    
    return sum % HASH_TABLE_SIZE;
}

void insertMedicine(Medicine* newMedicine){
    int key = hashFunction(newMedicine->medicineId);
    newMedicine->next = hashTable[key];
    hashTable[key] = newMedicine;
}

Medicine* searchMedicine(char* medicineId){
    int key = hashFunction(medicineId);
    Medicine* curr = hashTable[key];
    while(curr != NULL){
        if(strcmp(curr->medicineId, medicineId) == 0){
            return curr;
        }
        
        curr = curr->next;
    }
    
    return NULL;
}

void deleteMedicine(char* medicineId){
    int key = hashFunction(medicineId);
    Medicine* curr = hashTable[key];
    Medicine* previous = NULL;
    
    // debug
    //printf("DEBUG: Deleting ID = '%s'\n", medicineId);

    while(curr != NULL){
    	// debug
    	// printf("DEBUG: Checking against stored ID = '%s'\n", curr->medicineId);
    	
        if(strcmp(curr->medicineId, medicineId) == 0){
            if(previous == NULL){
                hashTable[key] = curr->next;
            } else{
                previous->next = curr->next;
            }
            free(curr);
            printf("Medicine deleted successfully!\n");
            return;
        }
        previous = curr;
        curr = curr->next;
    }
    
    printf("Medicine not found!\n");
}

void viewAllMedicines(){
    int isEmpty = 1;
    for(int i = 0; i < HASH_TABLE_SIZE; i++){
        Medicine* curr = hashTable[i];
        while(curr != NULL){
            puts("-------------------------------------------------------------------------------------------------------------");
            if(isEmpty){
                printf("| Medicine ID\t\t| Medicine Name\t\t| Manufacturer Name\t| Reg. No\t| Batch/Last Number |\n");
                isEmpty = 0;
            }
            puts("-------------------------------------------------------------------------------------------------------------");
            
            // debug
            // printf("DEBUG: Length of curr->medicineId = %ld\n", strlen(curr->medicineId));
            // printf("DEBUG: curr->medicineId = '%s'\n", curr->medicineId);
            
            printf("| %s\t| %s\t\t| %s\t\t| %s\t| %d\t\t   |\n", curr->medicineId, curr->medicineName, curr->manufacturerName, curr->registrationNo, curr->batchLotNumber);
            
            curr = curr->next;
        }
    }
    if(isEmpty){
        printf("There is no medicine(s)!\n");
    }
    printf("Press any key to continue . . .\n");
    getchar();
    getchar();
    system("cls");
}


void insertMedicineMenu(){
    Medicine* newMedicine = (Medicine*)malloc(sizeof(Medicine));
    char medicineName[MEDICINE_NAME_MAX + 1];
    char manufacturerName[MANUFACTURER_NAME_MAX + 1];
    char registrationNum[REGISTRATION_NUMBER_LENGTH + 1];
    char combinedInitials[3];
    int batchLotNumber;

    while(1){
        printf("Input Medicine Name[5-50][unique]: ");
		while (getchar() != '\n'); 
		fgets(medicineName, sizeof(medicineName), stdin);
		medicineName[strcspn(medicineName, "\n")] = 0;
        
        if(strlen(medicineName) < MEDICINE_NAME_MIN || strlen(medicineName) > MEDICINE_NAME_MAX){
            continue;
        }
        
        break;
    }

    while(1){
        printf("Input Manufacturer Name[3-25][end with Ltd.]: ");
        fgets(manufacturerName, sizeof(manufacturerName), stdin);
		manufacturerName[strcspn(manufacturerName, "\n")] = 0;
        
        if(strlen(manufacturerName) < MANUFACTURER_NAME_MIN || strlen(manufacturerName) > MANUFACTURER_NAME_MAX){
            continue;
        }
        if(strstr(manufacturerName, "Ltd.") == NULL){
            continue;
        }
        
        break;
    }

    while(1){
        printf("Input Registration No.[10 digits][numeric]: ");
        scanf("%s", registrationNum);
        
        if(strlen(registrationNum) != REGISTRATION_NUMBER_LENGTH){
            continue;
        }
        int valid = 1;
        for(int i = 0; i < REGISTRATION_NUMBER_LENGTH; i++){
            if(!isdigit(registrationNum[i])){
            	valid = 0;
                break;
            }
        }
        
        if(valid){
        	break;
		}
    }

    while(1){
        printf("Input Batch/Lot>= 16: ");
        scanf("%d", &batchLotNumber);
        if(batchLotNumber < BATCH_LOT_MIN) {
            continue;
        }
        
        break;
    }
    
    snprintf(combinedInitials, sizeof(combinedInitials), "%c%c", 
	         toupper(manufacturerName[0]), toupper(medicineName[0]));
	    
	// debug
	// printf("DEBUG: combinedInitials = %s\n", combinedInitials);

	lastMedicineIdNumber++;
	
	memset(newMedicine->medicineId, 0, sizeof(newMedicine->medicineId)); 	
 	snprintf(newMedicine->medicineId, sizeof(newMedicine->medicineId), 
         "M%05d-%s-%s", lastMedicineIdNumber, registrationNum, combinedInitials);
         
    newMedicine->medicineId[sizeof(newMedicine->medicineId) - 1] = '\0';
 	
 	// debug
 	// printf("DEBUG: medicineId = %s\n", newMedicine->medicineId);

    strcpy(newMedicine->medicineName, medicineName);
    strcpy(newMedicine->manufacturerName, manufacturerName);
    strcpy(newMedicine->registrationNo, registrationNum);
    newMedicine->batchLotNumber = batchLotNumber;
    newMedicine->next = NULL;

    insertMedicine(newMedicine);
    
    int index = hashFunction(newMedicine->medicineId) + 1;
    
    printf("Index: %d\n\n", index);
	
    printf("Insert Success!\n");
    printf("Press any key to continue . . .\n");
    getchar();
    getchar();
    system("cls");
}

void removeMedicineMenu(){
    char medicineId[25];
    printf("Input Medicine ID to delete: ");
    while (getchar() != '\n'); 
	fgets(medicineId, sizeof(medicineId), stdin);
	medicineId[strcspn(medicineId, "\n")] = 0;
    
    // debug
    // printf("DEBUG: Input Medicine ID = '%s'\n", medicineId);

    Medicine* medicineToDel = searchMedicine(medicineId);
    
    if(medicineToDel == NULL) {
        printf("Medicine not found!\n");
        printf("Press any key to continue . . .\n");
        getchar();
        getchar();
        system("cls");
        return;
    }

    printf("Medicine ID : %s\n", medicineToDel->medicineId);
    printf("Medicine Name : %s\n", medicineToDel->medicineName);
    printf("Manufacturer Name : %s\n", medicineToDel->manufacturerName);
    printf("Registration No. : %s\n", medicineToDel->registrationNo);
    printf("Batch/Lot : %d\n", medicineToDel->batchLotNumber);

    char confirmation;
    while(1){
        printf("Are you sure [y|n]? ");
        scanf(" %c", &confirmation);
        if(confirmation == 'y'){
            deleteMedicine(medicineId);
            break;
        } else if(confirmation == 'n'){
            printf("Deletion canceled.\n");
            break;
        } else{
            printf("Invalid input. Please enter 'y' or 'n'.\n");
        }
    }
    printf("Press any key to continue . . .\n");
    getchar();
    getchar();
    system("cls");
}

void menu(){
    int choice;
    while(1){
        printf("Sunib Hospital\n");
        printf("==============\n");
        printf("1. View Medicine\n");
        printf("2. Insert Medicine\n");
        printf("3. Remove Medicine\n");
        printf("4. Exit\n");
        printf(">> ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
            	system("cls");
                viewAllMedicines();
                break;
            case 2:
            	system("cls");
                insertMedicineMenu();
                break;
            case 3:
            	system("cls");
                removeMedicineMenu();
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main(){
    initHashTable();
    menu();
    return 0;
}
