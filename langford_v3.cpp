/**
 * Algorithme de Langford avec cas triviaux et symétries
 * @author Eliot CALD
 * @version 3.2
 */

#include <iostream>
#include <stack>
#include <vector>

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
    int j_invert = n-1;
    int taille_tab_verif = n*2;
    bool fail_verif = false;
    int cpt_solutions = 0; // compteur du nombre de solutions finales

    for(int i=0; i<n; i++){ // calcul du nb de solutions totale
        k *= ((n-1) + i);
    }

    cout << "Valeur de k : " << k << endl;

    

    // déclaration du tableau qui contiendra toutes les solutions
    int** tab_solutions_raw = (int**)malloc(sizeof(int*)*k);

    if (tab_solutions_raw == NULL) {
        cerr << "Erreur d'allocation mémoire pour tab_solutions_raw" << endl;
        exit(EXIT_FAILURE);
    }


    // initialisation du tab_solution
    for(int i=0; i<k; i++){
        tab_solutions_raw[i] = (int*)malloc(sizeof(int)*n);

        for(int j=0; j<n; j++){ // save de la solution dans le tableau de solutions
            tab_solutions_raw[i][j] = 0;
        }
    }

    int* tab_verif = (int*)malloc(sizeof(int)*taille_tab_verif); // tableau qui permet de verifier dans la boucle si c'est correct

    int* tab_range = (int*)malloc(sizeof(int)*n); // tableau de solutions individuelle

    int* tab_lock = (int*)malloc(sizeof(int)*n); // tableau pour savoir si la case peut avancer ou pas

    vector<int*> liste_solution;

    /* 
        Génération du tableau
    */

    // initialisation des tab_range et tab_lock
    for(int j=0; j<n; j++){
        tab_range[j] = 0;
        tab_lock[j] = 0;
    }

    

    for(int i=0; i<k; i++){
        
        j_invert = n-1;
        for(int j=0; j<n; j++){ // save de la solution dans le tableau de solutions
            tab_solutions_raw[i][j_invert] = tab_range[j];
            j_invert--;
        }

        tab_range[n-1] = ((tab_range[n-1]%(n+(n-3))) == 0 && tab_range[n-1] != 0)?0:tab_range[n-1]+1;

        for(int j=n-1; j>0; j--){
            if(tab_range[j] == 0 && tab_lock[j-1] == 1){

                tab_range[j-1] = ((tab_range[j-1]%(n+(j-n+(n-3)))) == 0 && tab_range[j-1] != 0)?0:tab_range[j-1]+1;

                tab_lock[j-1] = 0;

            }
            else if(tab_range[j] != 0){
                tab_lock[j-1] = 1;
            }
        }
    
    }



    // Verif des solution
    for (int i = 0; i < k; i++) {

        //reset
        for(int j=0; j<taille_tab_verif; j++){
            tab_verif[j] = 0;
        }

        fail_verif = false;


        for (int j = 0; j < n; j++) {
            if(tab_verif[tab_solutions_raw[i][j]] == 0 && tab_verif[tab_solutions_raw[i][j]+j+2] == 0){
                tab_verif[tab_solutions_raw[i][j]] = j+1;
                tab_verif[tab_solutions_raw[i][j]+j+2] = j+1;
            }
            else{
                fail_verif = true;
                break;
            }
        }

        if(!fail_verif){
            // liste_solution.push_back(tab_solutions_raw[i]);
            cpt_solutions++;
        }

    }


    // for (int* solution : liste_solution) {
    //     for(int i=0; i<n; i++){
    //         cout << solution[i] << " ";
    //     }
    //     cout << endl;
    // }

    cout << "Solutions >> " << cpt_solutions << endl;


/*
    // Affichage du tableau
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            cout << tab_solutions_raw[i][j] << " ";
        }
        cout << endl;
    }
*/

    // Libération de la mémoire
    for (int i = 0; i < k; i++) {
        free(tab_solutions_raw[i]);
    }
    free(tab_solutions_raw);
    free(tab_range);
    free(tab_lock);


    return EXIT_SUCCESS;
}//fin main