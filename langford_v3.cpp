/**
 * Algorithme de Langford
 * @author Eliot CALD
 * @version 3.1
 */

#include <iostream>
#include <stack>

using namespace std;

#define NB_PARAM 1

int main(int argc, char* argv[]){

    if(argc != NB_PARAM + 1){
        cerr << "Paramètre : valeur de N" << endl;
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    int k = 1; // nb de solutions possible avec cas triviaux
    int val = 0; // valeur à placer dans la case
    int taille_tab_verif = n*2;

    for(int i=0; i<n; i++){ // calcul du nb de solutions totale
        k *= ((n-1) + i);
    }

    cout << "Valeur de k : " << k << endl;

    

    // déclaration du tableau qui contiendra toutes les solutions
    int** tab_solutions = (int**)malloc(sizeof(int*)*k);

    if (tab_solutions == NULL) {
        cerr << "Erreur d'allocation mémoire pour tab_solutions" << endl;
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<k; i++){
        tab_solutions[i] = (int*)malloc(sizeof(int)*n);
        for(int j=0; j<n; j++){ // save de la solution dans le tableau de solutions
            tab_solutions[i][j] = 0;
        }
    }

    int* tab_verif = (int*)malloc(sizeof(int)*taille_tab_verif); // tableau qui permet de verifier dans la boucle si c'est correct

    int* tab_range = (int*)malloc(sizeof(int)*n); // tableau de solutions individuelle

    /* 
        Génération du tableau
    */

    for(int j=0; j<n; j++){
        tab_range[j] = 0;
    }

    for(int i=0; i<k; i++){
        
        for(int j=0; j<n; j++){ // save de la solution dans le tableau de solutions
            tab_solutions[i][j] = tab_range[j];
        }

        tab_range[n-1]++;
        
        for(int j=n-2; j>=0; j--){
            if(tab_range[j]%((n-1)+(j-1)) == 0){
                tab_range[j] = 0;
                tab_range[j-1]++;
            }
        }
    }


    // Affichage du tableau
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            cout << tab_solutions[i][j] << " ";
        }
        cout << endl;
    }



    // Libération de la mémoire
    for (int i = 0; i < k; i++) {
        free(tab_solutions[i]);
    }
    free(tab_solutions);


    return EXIT_SUCCESS;
}//fin main