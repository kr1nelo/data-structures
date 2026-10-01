#include <stdio.h>
#include <stdlib.h>

#define size 3

struct Barang{
	char idItem[6];
	int stock;
};

void view(struct Barang *ptr);
void sell(struct Barang *ptr);
void add(struct Barang *ptr);

int main(){
	struct Barang arr[3];
	
	strcpy(arr[0].idItem, "ID001");
	strcpy(arr[1].idItem, "ID002");
	strcpy(arr[2].idItem, "ID003");
	
	arr[0].stock = 6;
	arr[1].stock = 10;
	arr[2].stock = 15;
	
	int input;
	do{
		system("cls");
		printf("Menu\n");
		printf("====\n");
		printf("1. View Item\n");
		printf("2. Sell\n");
		printf("3. Add Stock\n");
		printf("Input : ");
		
		switch(input){
			case 1:
				view(arr);
				printf("Press any button to continue\n");
				getch();
				break;
			case 2:
				sell(arr);
				printf("Press any button to continue\n");
				getch();
				break;
			case 3:
				add(arr);
				printf("Press any button to continue\n");
				getch();
				break;
			case 4:
				printf("Thank you for using my program");
			default:
				printf("Invalid input\n");
				getch();
				break;
		} while(input != 4);
		
		return 0;
	}
	
	void view(struct Barang *ptr){
		system("cls");
		printf("Nama Barang | Stock |\n");
		printf("======================\n");
		for(int i = 0; i < size; i++){
			printf("%-11s | %-5d |\n, ptr[i].idItem, ptr[i].stock");
			printf("=============================================\n");
		}
	}
	
//	void sell(struct Barang *ptr){
//		system("cls");
//		char itemID[6];
//		int quantity;
	}
}
