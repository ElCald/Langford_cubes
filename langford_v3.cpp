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
    int n_up = n+1; //valeur n mais augmenter pour générer les solutions
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
        tab_solutions[i] = (int*)malloc(sizeof(int)*n_up);
    }

    int* tab_verif = (int*)malloc(sizeof(int)*taille_tab_verif); // tableau qui permet de verifier dans la boucle si c'est correct

    // Plages de valeurs pour chaque colonne
    int* ranges = (int*)malloc(sizeof(int)*n_up);
    for (int i = 0; i < n_up; i++) {
        ranges[i] = n_up - 2 + i;  // La plage de chaque colonne augmente
    }

    // Génération des combinaisons
    for (int i = 0; i < k; i++) {
        val = i;
        for (int j = 0; j < n_up; j++) {
            tab_solutions[i][j] = val % ranges[j];  // Affecter la valeur de la colonne
            val /= ranges[j];  // Mise à jour de l'index temporaire
        }
    }


    // Affichage du tableau
    // for (int i = 0; i < k; i++) {
    //     for (int j = 0; j < n-1; j++) {
    //         cout << tab_solutions[i][j] << " ";
    //     }
    //     cout << endl;
    // }


    /*
        Toutes les combinaisons sont générés, il faut maintenant garder tout tester et ne sauvegarder que celle qui sont correctes
    */

   for (int i = 0; i < k; i++) {        

        int n_insert;
        bool fail = false;

        for(int a=0; a<taille_tab_verif; a++){
            tab_verif[a] = 0;
        }


        for (int j = 0; j < n; j++) {
            n_insert = tab_solutions[i][j]; // indice à placer

            if(tab_verif[n_insert] == 0 && tab_verif[n_insert+n+1] == 0){ // Vérification dans le tableau

                tab_verif[n_insert] = j;
                tab_verif[n+n_insert+1] = j;
            }
            else{
                fail = true;
                break;
            }
        }


        if(fail == false){
            // afficher la ligne i du tableau
            for(int ii=0; ii<n; ii++){
                cout << tab_solutions[i][ii] << " ";
            }
            cout << endl;
        }

        
    }


    // Libération de la mémoire
    for (int i = 0; i < k; i++) {
        free(tab_solutions[i]);
    }
    free(tab_solutions);
    free(ranges);
    free(tab_verif);

    return EXIT_SUCCESS;
}//fin main