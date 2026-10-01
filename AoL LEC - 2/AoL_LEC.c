#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PLAYERS 100

// Struktur untuk pemain
typedef struct {
    char name[50];
    int score;
    char matchWL[10]; // Format: 5-3
    char gameWL[10];  // Format: 23-12
    int netGameWin;
} Player;

// Fungsi untuk memperbaiki max-heap dari bawah ke atas
void heapifyUp(Player heap[], int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].score > heap[parent].score) {
            Player temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;
            index = parent;
        } else {
            break;
        }
    }
}

// Fungsi untuk memperbaiki max-heap dari atas ke bawah
void heapifyDown(Player heap[], int size, int index) {
    while (2 * index + 1 < size) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int largest = index;
        
        if (leftChild < size && heap[leftChild].score > heap[largest].score) {
            largest = leftChild;
        }
        if (rightChild < size && heap[rightChild].score > heap[largest].score) {
            largest = rightChild;
        }
        if (largest != index) {
            Player temp = heap[index];
            heap[index] = heap[largest];
            heap[largest] = temp;
            index = largest;
        } else {
            break;
        }
    }
}

// Validasi Nama Team
int isValidTeamName(const char *name) {
    int length = strlen(name);
    if (length < 2 || length > 4) return 0;
    for (int i = 0; i < length; i++) {
        if (!isupper(name[i])) return 0;
    }
    return 1;
}

int isValidWLFormat(const char *wl, char *cleanedWL) {
    int len = strlen(wl);
    int dashCount = 0;
    int dashPos = -1;
    int cleanIndex = 0;
    
    // First pass to validate and find dash position
    for (int i = 0; i < len; i++) {
        if (wl[i] == '-') {
            dashCount++;
            dashPos = i;
            if (dashCount > 1) return 0; // More than one dash
        } else if (!isdigit(wl[i]) && !isspace(wl[i])) {
            return 0; // Invalid character
        }
    }
    
    if (dashCount != 1) return 0; // No dash or multiple dashes
    
    // Second pass to create cleaned version (without spaces around dash)
    int beforeDash = 1;
    for (int i = 0; i < len; i++) {
        if (i < dashPos && isspace(wl[i])) {
            continue; // Skip spaces before dash
        } else if (i > dashPos && isspace(wl[i])) {
            continue; // Skip spaces after dash
        } else {
            cleanedWL[cleanIndex++] = wl[i];
        }
    }
    cleanedWL[cleanIndex] = '\0';
    
    // Final validation of cleaned format
    len = strlen(cleanedWL);
    if (len < 3 || len > 9) return 0; // Too short or too long
    
    return 1;
}

void addPlayer(Player heap[], int *size) {
    char name[50];
    char matchWL[10];
    char gameWL[10];
    char cleanedMatchWL[10];
    char cleanedGameWL[10];
    int matchWin, matchLoss, gameWin, gameLoss;
    int isDuplicate;
    
    printf("=== TAMBAHKAN TEAM ===\n");
    printf("Note:\n");
    printf("*Ketik '0' untuk membatalkan dan kembali ke menu awal\n");
    printf("*Nama team harus 2-4 huruf kapital tanpa angka atau karakter khusus.\n");
    printf("*Maksimal 16 match total\n");
    printf("*Gunakan format Win-Lose, contoh: 0-0 atau 0 - 0.\n");
    printf("\n");
    
    // Input Nama Team dengan Opsi Batal
    do {
        printf("Masukkan nama team: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        if (strcmp(name, "0") == 0) {
            system("cls");
            return;
        }
        if (!isValidTeamName(name)) {
            continue;
        }
        isDuplicate = 0;
        for (int i = 0; i < *size; i++) {
            if (strcmp(heap[i].name, name) == 0) {
                printf("Nama team '%s' sudah ada. Tidak boleh duplikat.\n", name);
                isDuplicate = 1;
                break;
            }
        }
    } while (!isValidTeamName(name) || isDuplicate);
    
    // Validasi Match W-L (maksimal 16 match)
    do {
        printf("Masukkan Match W-L (format: W-L): ");
        fgets(matchWL, sizeof(matchWL), stdin);
        matchWL[strcspn(matchWL, "\n")] = 0;
        if (strcmp(matchWL, "0") == 0) {
            printf("Penambahan team dibatalkan.\n");
            return;
        }
        if (!isValidWLFormat(matchWL, cleanedMatchWL)) {
            printf("Format Match W-L tidak valid. Gunakan format W-L, contoh: 5-3.\n");
            continue;
        }
        strcpy(matchWL, cleanedMatchWL); // Use the cleaned version
        sscanf(matchWL, "%d-%d", &matchWin, &matchLoss);
        if (matchWin + matchLoss > 16) {
            printf("Total match tidak boleh lebih dari 16.\n");
            continue;
        }
    } while (!isValidWLFormat(matchWL, cleanedMatchWL) || (matchWin + matchLoss) > 16);
    
    // Validasi Game W-L berdasarkan logika BO3
    do {
        printf("Masukkan Game W-L (format: W-L): ");
        fgets(gameWL, sizeof(gameWL), stdin);
        gameWL[strcspn(gameWL, "\n")] = 0;
        if (strcmp(gameWL, "0") == 0) {
            printf("Penambahan team dibatalkan.\n");
            return;
        }
        if (!isValidWLFormat(gameWL, cleanedGameWL)) {
            printf("Format Game W-L tidak valid. Gunakan format W-L, contoh: 23-12.\n");
            continue;
        }
        strcpy(gameWL, cleanedGameWL); // Use the cleaned version
        sscanf(gameWL, "%d-%d", &gameWin, &gameLoss);
        
        int totalMatch = matchWin + matchLoss;
        int totalGame = gameWin + gameLoss;
        
        // 1. Validasi total game (BO3: 2-3 game per match)
        if (totalGame < 2 * totalMatch || totalGame > 3 * totalMatch) {
            printf("Total Game (W+L) harus antara %d dan %d untuk %d match BO3\n", 
                  2 * totalMatch, 3 * totalMatch, totalMatch);
            continue;
        }
        
        // 2. Validasi Game Win (untuk match yang dimenangkan: 2-3 game per match)
        if (matchWin > 0) {
            if (gameWin < 2 * matchWin || gameWin > 3 * matchWin) {
                printf("Game Win harus antara %d dan %d untuk %d Match Win\n",
                      2 * matchWin, 3 * matchWin, matchWin);
                continue;
            }
        } else {
            // Jika tidak ada match win, game win bisa antara 0 dan (totalGame - 2*matchLoss)
            if (gameWin < 0 || gameWin > (totalGame - 2 * matchLoss)) {
                printf("Game Win harus antara 0 dan %d untuk 0 Match Win\n",
                      (totalGame - 2 * matchLoss));
                continue;
            }
        }
        
        // 3. Validasi Game Loss (minimal 2 game per match kalah)
        if (gameLoss < 2 * matchLoss || gameLoss > (totalGame - (matchWin > 0 ? 2 * matchWin : 0))) {
            printf("Game Loss harus antara %d dan %d untuk %d Match Loss\n",
                  2 * matchLoss, (totalGame - (matchWin > 0 ? 2 * matchWin : 0)), matchLoss);
            continue;
        }
        
        break;
    } while (1);
    
    // Hitung Net Game Win dan Simpan Data
    int netGameWin = gameWin - gameLoss;
    strcpy(heap[*size].name, name);
    strcpy(heap[*size].matchWL, matchWL);
    strcpy(heap[*size].gameWL, gameWL);
    heap[*size].netGameWin = netGameWin;
    heap[*size].score = matchWin; // Match Point = Match Win
    (*size)++;
    
    printf("Pemain %s berhasil ditambahkan.\n", name);
    printf("Tekan Enter untuk kembali ke menu utama...");
    getchar();
    system("cls");
}

// Menampilkan pemain dengan skor tertinggi dalam format tabel
void getTopPlayer(Player heap[], int size) {
    if (size == 0) {
        printf("Tidak ada pemain dalam peringkat.\n");
        return;
    }
    
    Player topPlayer = heap[0];
    
    printf("\n============================== PERINGKAT TERTINGGI ================================\n");
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("|        TEAM          | MATCH POINT  |  MATCH W-L  | NET GAME WIN |   GAME W-L   |\n");
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("| %-20s |      %2d      |   %-9s |      %3d     |   %-9s  |\n",
           topPlayer.name, topPlayer.score, topPlayer.matchWL, topPlayer.netGameWin, topPlayer.gameWL);
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("Tekan enter untuk kembali ke menu utama...");
    getchar();
    system("cls");
}

void removePlayerByName(Player heap[], int *size) {
    if (*size == 0) {
        printf("Tidak ada pemain untuk dihapus.\n");
        return;
    }
    
    char targetName[50];
    int foundIndex = -1;
    
    // Meminta input nama team
    printf("Masukkan nama team yang akan dihapus (atau ketik 0 untuk batal): ");
    fgets(targetName, sizeof(targetName), stdin);
    targetName[strcspn(targetName, "\n")] = 0;
    
    if (strcmp(targetName, "0") == 0) {
        printf("Penghapusan team dibatalkan.\n");
        return;
    } else{
    // Mencari team dalam heap
    for (int i = 0; i < *size; i++) {
        if (strcmp(heap[i].name, targetName) == 0) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex == -1) {
        printf("Team '%s' tidak ditemukan dalam peringkat.\n", targetName);
        return;
    }
    
    // Menampilkan data team yang akan dihapus
    Player removedPlayer = heap[foundIndex];
    printf("\n=============================== TEAM YANG DIHAPUS =================================\n");
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("|        TEAM          | MATCH POINT  |  MATCH W-L  | NET GAME WIN |   GAME W-L   |\n");
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("| %-20s |      %2d      |   %-9s |      %3d     |   %-9s  |\n",
           removedPlayer.name, removedPlayer.score, removedPlayer.matchWL, removedPlayer.netGameWin, removedPlayer.gameWL);
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    
    // Menghapus team dari heap dan memperbaiki struktur heap
    heap[foundIndex] = heap[(*size) - 1];
    (*size)--;
    heapifyDown(heap, *size, foundIndex);
    
    printf("Pemain %s telah dihapus dari peringkat.\n", removedPlayer.name);
	}	
}    
    
void displayAllPlayers(Player heap[], int size) {
    if (size == 0) {
        printf("Tidak ada pemain dalam peringkat.\n");
        return;
    }
    
    // Mengurutkan pemain berdasarkan Match Point, Match W-L, Net Game Win, dan Game W-L
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            // Prioritas 1: Match Point (score)
            if (heap[j].score < heap[j+1].score ||
                // Prioritas 2: Match W-L (lebih banyak win lebih tinggi)
               (heap[j].score == heap[j+1].score && strcmp(heap[j].matchWL, heap[j+1].matchWL) < 0) ||
                // Prioritas 3: Net Game Win
               (heap[j].score == heap[j+1].score && strcmp(heap[j].matchWL, heap[j+1].matchWL) == 0 && heap[j].netGameWin < heap[j+1].netGameWin) ||
                // Prioritas 4: Game W-L
               (heap[j].score == heap[j+1].score && strcmp(heap[j].matchWL, heap[j+1].matchWL) == 0 && heap[j].netGameWin == heap[j+1].netGameWin && strcmp(heap[j].gameWL, heap[j+1].gameWL) < 0)) {
                Player temp = heap[j];
                heap[j] = heap[j+1];
                heap[j+1] = temp;
            }
        }
    }
    
    printf("\n=================================== DAFTAR KLASEMEN ====================================\n");
    printf("+----+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("| NO |        TEAM          |  MATCH POINT |  MATCH W-L  | NET GAME WIN |   GAME W-L   |\n");
    printf("+----+----------------------+--------------+-------------+--------------+--------------+\n");
    
    for (int i = 0; i < size; i++) {
        printf("| %2d | %-20s |      %2d      |   %-9s |      %3d     |   %-9s  |\n",
               i + 1, heap[i].name, heap[i].score, heap[i].matchWL, heap[i].netGameWin, heap[i].gameWL);
    }
    
    printf("+----+----------------------+--------------+-------------+--------------+--------------+\n");
}

// Function to update a player's score
void updatePlayerScore(Player heap[], int size) {
    if (size == 0) {
        printf("Tidak ada pemain dalam peringkat.\n");
        return;
    }

    char targetName[50];
    char matchWL[10];
    char gameWL[10];
    char cleanedMatchWL[10];
    char cleanedGameWL[10];
    int foundIndex = -1;
    int matchWin, matchLoss, gameWin, gameLoss;
    
    printf("=== UPDATE SCORE TEAM ===\n");
    printf("Note:\n");
    printf("*Ketik '0' untuk membatalkan dan kembali ke menu utama\n");
    printf("*Maksimal 16 match total\n");
    printf("*Gunakan format Win-Lose, contoh: 0-0 atau 0 - 0.\n");
    printf("\n");
    
    // Input nama team yang akan diupdate dengan opsi input ulang jika tidak ditemukan
    do {
        printf("Masukkan nama team yang akan diupdate: ");
        fgets(targetName, sizeof(targetName), stdin);
        targetName[strcspn(targetName, "\n")] = 0;
        
        if (strcmp(targetName, "0") == 0) {
            system("cls");
            return;
        }
        
        // Cari team dalam heap
        foundIndex = -1;
        for (int i = 0; i < size; i++) {
            if (strcmp(heap[i].name, targetName) == 0) {
                foundIndex = i;
                break;
            }
        }
        
        if (foundIndex == -1) {
            printf("Team '%s' tidak ditemukan dalam peringkat. Silakan coba lagi.\n", targetName);
        }
    } while (foundIndex == -1);
    
    // Tampilkan data team saat ini
    Player currentPlayer = heap[foundIndex];
    printf("Data team saat ini:\n");
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("|        TEAM          | MATCH POINT  |  MATCH W-L  | NET GAME WIN |   GAME W-L   |\n");
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("| %-20s |      %2d      |   %-9s |      %3d     |   %-9s  |\n",
           currentPlayer.name, currentPlayer.score, currentPlayer.matchWL, 
           currentPlayer.netGameWin, currentPlayer.gameWL);
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    
    // Validasi Match W-L (maksimal 16 match)
    do {
        printf("\nMasukkan Match W-L baru: ");
        fgets(matchWL, sizeof(matchWL), stdin);
        matchWL[strcspn(matchWL, "\n")] = 0;
        if (strcmp(matchWL, "0") == 0) {
            system("cls");
            return;
        }
        if (!isValidWLFormat(matchWL, cleanedMatchWL)) {
            printf("Format Match W-L tidak valid\n");
            continue;
        }
        strcpy(matchWL, cleanedMatchWL); // Use the cleaned version
        sscanf(matchWL, "%d-%d", &matchWin, &matchLoss);
        if (matchWin + matchLoss > 16) {
            printf("Total match tidak boleh lebih dari 16.\n");
            continue;
        }
    } while (!isValidWLFormat(matchWL, cleanedMatchWL) || (matchWin + matchLoss) > 16);
    
    // Validasi Game W-L berdasarkan logika BO3
    do {
        printf("Masukkan Game W-L baru: ");
        fgets(gameWL, sizeof(gameWL), stdin);
        gameWL[strcspn(gameWL, "\n")] = 0;
        if (strcmp(gameWL, "0") == 0) {
            system("cls");
            return;
        }
        if (!isValidWLFormat(gameWL, cleanedGameWL)) {
            printf("Format Game W-L tidak valid\n");
            continue;
        }
        strcpy(gameWL, cleanedGameWL); // Use the cleaned version
        sscanf(gameWL, "%d-%d", &gameWin, &gameLoss);
        
        int totalMatch = matchWin + matchLoss;
        int totalGame = gameWin + gameLoss;
        
        // 1. Validasi total game (BO3: 2-3 game per match)
        if (totalGame < 2 * totalMatch || totalGame > 3 * totalMatch) {
            printf("Total Game (W+L) harus antara %d dan %d untuk %d match BO3\n", 2 * totalMatch, 3 * totalMatch, totalMatch);
            continue;
        }
        
        // 2. Validasi Game Win (untuk match yang dimenangkan: 2-3 game per match)
        if (matchWin > 0) {
            if (gameWin < 2 * matchWin || gameWin > 3 * matchWin) {
                printf("Game Win harus antara %d dan %d untuk %d Match Win\n", 2 * matchWin, 3 * matchWin, matchWin);
                continue;
            }
        } else {
            // Jika tidak ada match win, game win bisa antara 0 dan (totalGame - 2*matchLoss)
            if (gameWin < 0 || gameWin > (totalGame - 2 * matchLoss)) {
                printf("Game Win harus antara 0 dan %d untuk 0 Match Win\n", (totalGame - 2 * matchLoss));
                continue;
            }
        }
        
        // 3. Validasi Game Loss (minimal 2 game per match kalah)
        if (gameLoss < 2 * matchLoss || gameLoss > (totalGame - (matchWin > 0 ? 2 * matchWin : 0))) {
            printf("Game Loss harus antara %d dan %d untuk %d Match Loss\n", 2 * matchLoss, (totalGame - (matchWin > 0 ? 2 * matchWin : 0)), matchLoss);
            continue;
        }
        
        break;
    } while (1);
    
    // Update data team
    int netGameWin = gameWin - gameLoss;
    strcpy(heap[foundIndex].matchWL, matchWL);
    strcpy(heap[foundIndex].gameWL, gameWL);
    heap[foundIndex].netGameWin = netGameWin;
    heap[foundIndex].score = matchWin; // Match Point = Match Win
    
    // Perbaiki heap setelah update
    heapifyUp(heap, foundIndex);
    heapifyDown(heap, size, foundIndex);
    
    printf("\nData team %s berhasil diupdate:\n", targetName);
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("|        TEAM          | MATCH POINT  |  MATCH W-L  | NET GAME WIN |   GAME W-L   |\n");
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    printf("| %-20s |      %2d      |   %-9s |      %3d     |   %-9s  |\n",
           heap[foundIndex].name, heap[foundIndex].score, heap[foundIndex].matchWL, 
           heap[foundIndex].netGameWin, heap[foundIndex].gameWL);
    printf("+----------------------+--------------+-------------+--------------+--------------+\n");
    
    printf("Tekan Enter untuk kembali ke menu utama...");
    getchar();
    system("cls");
}

void resetRanking(Player heap[], int *size) {
    char confirmation[10];
    
    printf("=== RESET KLASEMEN ===\n");
    
    // Show current standings first
    if (*size == 0) {
        printf("\nKlasemen saat ini kosong.\n");
    } else {
        printf("\nKlasemen saat ini:\n");
        displayAllPlayers(heap, *size);  // Reuse the existing display function
    }
    
    printf("\nApakah Anda yakin ingin mereset seluruh klasemen?\n");
    printf("1. Ya, reset klasemen\n");
    printf("0. Batal, kembali ke menu utama\n");
    
    do {
        printf("\nPilihan Anda (1/0): ");
        fgets(confirmation, sizeof(confirmation), stdin);
        confirmation[strcspn(confirmation, "\n")] = '\0';
        
        if (strcmp(confirmation, "1") == 0) {
            *size = 0;
            printf("\nPeringkat telah direset.\n");
            printf("Tekan Enter untuk kembali ke menu utama...");
            getchar();
            system("cls");
            return;
        } 
        else if (strcmp(confirmation, "0") == 0) {
            system("cls");
            return;
        }
        else {
            printf("Input tidak valid. Masukkan 1 untuk reset atau 0 untuk batal.");
        }
    } while (1);
}

int main() {
    Player playerHeap[MAX_PLAYERS];
    int size = 0;
    int choice;
    
    do {
        printf("\n=== Sistem Klasemen MPL ID ===\n");
        printf("1. Tambahkan Team\n");
        printf("2. Lihat peringkat tertinggi klasemen\n");
        printf("3. Hapus Team berdasarkan nama\n");
        printf("4. Tampilkan Klasemen\n");
        printf("5. Update Score Team\n");
        printf("6. Reset Klasemen\n");
        printf("7. Keluar\n");
        printf(">> ");
        scanf("%d", &choice);
        getchar();
        
        switch (choice) {
            case 1:
            	system("cls");
                addPlayer(playerHeap, &size);
                break;
            case 2:
            	system("cls");
                getTopPlayer(playerHeap, size);
                break;
            case 3:
            	system("cls");
                removePlayerByName(playerHeap, &size);
                break;
            case 4:
            	system("cls");
                displayAllPlayers(playerHeap, size);
                break;
            case 5:
            	system("cls");
            	updatePlayerScore(playerHeap, size);
            	break;
            case 6:
            	system("cls");
                resetRanking(playerHeap, &size);
                break;
            case 7:
            	printf("Terima kasih dan sampai jumpa\n");
                break;
            default:
            	system("cls");
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (choice != 7);
    return 0;
}
