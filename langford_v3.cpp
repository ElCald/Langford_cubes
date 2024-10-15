/**
 * Algorithme de Langford avec cas triviaux et symétries
 * @author Eliot CALD
 * @version 3.2
 */

#include <iostream>
#include <stack>
#include <vector>
#include <omp.h>
#include <algorithm>

using namespace std;

#define NB_PARAM 1


/**
 * Elimine les cas trivaux, donc si on retrouve 2 fois la même valeur dans le tableau
 * A voir pour retirer les valeurs qui sont adjacente et qui ont juste une case d'écart
 * @param tab Tableau tab_range
 * @param taille Taille du tableau
 * @return True s'il y a un cas triviale et false dans le cas où c'est conforme
 */
bool cas_triviaux(int* tab, int taille){

    for(int i=0; i<taille; i++){
        
        for(int j=0; j<taille; j++){
            
            if(tab[i] == tab[j] && i!=j){
                return true;
            }
        }
    }

    return false;
}// fin cas_triviaux


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
    int cpt_no_trivaux = 0; // compteur de solutions non triviales
    int n_val = n; // variable qui permet le calcul de la valeur du modulo qui est décémenté

    for(int i=0; i<n; i++){ // calcul du nb de solutions totale
        k *= ((n-1) + i);
    }

    cout << "Solutions totales >> " << k << endl;

    

    // déclaration du tableau qui contiendra toutes les solutions
    // int** tab_solutions_raw = (int**)malloc(sizeof(int*)*k);

    // if (tab_solutions_raw == NULL) {
    //     cerr << "Erreur d'allocation mémoire pour tab_solutions_raw" << endl;
    //     exit(EXIT_FAILURE);
    // }


    // initialisation du tab_solution
    // for(int i=0; i<k; i++){
    //     tab_solutions_raw[i] = (int*)malloc(sizeof(int)*n);

    //     for(int j=0; j<n; j++){ // save de la solution dans le tableau de solutions
    //         tab_solutions_raw[i][j] = 0;
    //     }
    // }

    // cout << "Tableau initialisé" << endl;


    int* tab_verif = (int*)malloc(sizeof(int)*taille_tab_verif); // tableau qui permet de verifier dans la boucle si c'est correct

    int* tab_range = (int*)malloc(sizeof(int)*n); // tableau de solutions individuelle

    int* tab_lock = (int*)malloc(sizeof(int)*n); // tableau pour savoir si la case peut avancer ou pas

    vector<vector<int>> liste_solution;


    /* 
        Génération du tableau de solution
    */

    // initialisation des tab_range et tab_lock
    for(int j=0; j<n; j++){
        tab_range[j] = 0;
        tab_lock[j] = 0;
    }


    for(int i=0; i<k; i++){
     
        //IL FAUT REVERSE LE TABLEAU !!!!!!
        if(cas_triviaux(tab_range, n) == false){
            liste_solution.push_back(vector<int>(tab_range, tab_range + n));
            cpt_no_trivaux++;
        }
        
        n_val = n-1;

        // changer ici en indice 0
        tab_range[0] = ((tab_range[0]%(n+(n-3))) == 0 && tab_range[0] != 0)?0:tab_range[0]+1;

        //commencer à 1 et trouver un autre indice
        for(int j=1; j<n; j++){

            if(tab_range[j-1] == 0 && tab_lock[j] == 1){

                tab_range[j] = ((tab_range[j]%(n+(n_val-n+(n-3)))) == 0 && tab_range[j] != 0)?0:tab_range[j]+1;

                tab_lock[j] = 0;

            }
            else if(tab_range[j-1] != 0){
                tab_lock[j] = 1;
            }

            n_val--;
        }



/*  BACKUP
        // changer ici en indice 0
        tab_range[n-1] = ((tab_range[n-1]%(n+(n-3))) == 0 && tab_range[n-1] != 0)?0:tab_range[n-1]+1;

        //commencer à 0 et trouver un autre indice
        for(int j=n-1; j>0; j--){
            if(tab_range[j] == 0 && tab_lock[j-1] == 1){

                tab_range[j-1] = ((tab_range[j-1]%(n+(j-n+(n-3)))) == 0 && tab_range[j-1] != 0)?0:tab_range[j-1]+1;

                tab_lock[j-1] = 0;

            }
            else if(tab_range[j] != 0){
                tab_lock[j-1] = 1;
            }
        }
*/    
    }
    



    // Verif des solution

    //version tableau
/*    #pragma omp for private(fail_verif, tab_verif)
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
            #pragma omp critical
            cpt_solutions++;
        }
    }

*/

    //version liste
    #pragma omp for private(fail_verif, tab_verif)
    for (vector<int> solution : liste_solution) {

        //reset
        for(int j=0; j<taille_tab_verif; j++){
            tab_verif[j] = 0;
        }

        fail_verif = false;

         
        if(!solution.empty()){
            for (int j = 0; j < n; j++) {
                if(tab_verif[solution.at(j)] == 0 && tab_verif[solution.at(j)+j+2] == 0){
                    tab_verif[solution.at(j)] = j+1;
                    tab_verif[solution.at(j)+j+2] = j+1;
                }
                else{
                    fail_verif = true;
                    break;
                }
            }
        }

        if(!fail_verif){
            // liste_solution.push_back(tab_solutions_raw[i]);
            #pragma omp critical
            cpt_solutions++;
        }
        
    }





    // for (vector<int> solution : liste_solution) {
    //     for(int i=0; i<n; i++){
    //         cout << solution.at(i) << " ";
    //     }
    //     cout << endl;
    // }


    cout << "Solutions non triviales >> " << cpt_no_trivaux << endl;
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
    // for (int i = 0; i < k; i++) {
    //     free(tab_solutions_raw[i]);
    // }
    // free(tab_solutions_raw);
    // free(tab_range);
    // free(tab_lock);


    return EXIT_SUCCESS;
}//fin main