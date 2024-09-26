#include <stdio.h>
#include <stdlib.h>

void generate_table(int N) {
    int num_columns = N - 1;
    int num_rows = 1;
    
    // Calcul du nombre total de lignes
    for (int i = 0; i < num_columns; i++) {
        num_rows *= (N - 2 + i);  // Produit des valeurs de chaque plage
    }
    
    // Allocation dynamique de mémoire pour le tableau
    int **table = (int **)malloc(num_rows * sizeof(int *));
    for (int i = 0; i < num_rows; i++) {
        table[i] = (int *)malloc(num_columns * sizeof(int));
    }

    // Plages de valeurs pour chaque colonne
    int ranges[num_columns];
    for (int i = 0; i < num_columns; i++) {
        ranges[i] = N - 2 + i;  // La plage de chaque colonne augmente
    }

    // Génération des combinaisons
    for (int row = 0; row < num_rows; row++) {
        int temp = row;
        for (int col = 0; col < num_columns; col++) {
            table[row][col] = temp % ranges[col];  // Affecter la valeur de la colonne
            temp /= ranges[col];  // Mise à jour de l'index temporaire
        }
    }

    // Affichage du tableau
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_columns; j++) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }

    // Libération de la mémoire
    for (int i = 0; i < num_rows; i++) {
        free(table[i]);
    }
    free(table);
}

int main() {
    int N = 6;  // Exemple avec N = 4 (modifiable)
    generate_table(N);
    return 0;
}
