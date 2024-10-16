/**
 * Algorithme de Langford sans cas triviaux et avec symétries
 * @author Eliot CALD
 * @version 3.3
 */

#include <iostream>
#include <stack>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <time.h>

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
    unsigned long long k = 1; // nb de solutions possible avec cas triviaux
    int val = 0; // valeur à placer dans la case
    int j_invert = n-1;
    int taille_tab_verif = n*2;
    bool fail_verif = false;
    unsigned long long cpt_solutions = 0; // compteur du nombre de solutions finales
    unsigned long long cpt_no_trivaux = 0; // compteur de solutions non triviales
    int n_val = n; // variable qui permet le calcul de la valeur du modulo qui est décémenté

    clock_t start, stop, mid_time;
    double start_p, stop_p, mid_time_p, CPU_time, total;


    #ifndef _OPENMP
        cout << "Execution séquentielle..." << endl;
        start = clock();
    #else
        cout << "Execution parallèle..." << endl;
        start_p = omp_get_wtime();
    #endif


    for(int i=0; i<n; i++){ // calcul du nb de solutions totale
        k *= ((n-1) + i);
    }

    cout << "Solutions totales >> " << k << endl;

    



    vector<vector<int>> liste_solution; // liste qui contiendra toutes les solutions


    /* 
        Génération du tableau de solution
    */


    // tableau contenant la position de l'élément n pour permettre le threading
    #pragma omp parallel private(n_val)
    {
        #pragma omp for 
        for(int i=0; i<n-1; i++){

            int* tab_range = new int[n](); // tableau de solutions individuelle
            int* tab_lock = new int[n](); // tableau pour savoir si la case peut avancer ou pas

            tab_range[n-1] = i;
            
            while(tab_range[n-1] == i){

                if(cas_triviaux(tab_range, n) == false){
                    #pragma critical
                    liste_solution.push_back(vector<int>(tab_range, tab_range + n));
                    #pragma critical
                    cpt_no_trivaux++;
                }

                n_val = n-1;
                
                tab_range[0] = ((tab_range[0]%(n+(n-3))) == 0 && tab_range[0] != 0)?0:tab_range[0]+1;

                
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
            }
        }
    }

    #ifndef _OPENMP
        mid_time = clock();
        total = (double)mid_time - start;
        CPU_time = total / CLOCKS_PER_SEC;
    #else
        mid_time_p = omp_get_wtime();
        CPU_time = mid_time_p - start_p;
    #endif

    printf("Time fin init solutions : %lf seconds\n", CPU_time);

    if(!liste_solution.empty()){
        cout << "liste non vide" << endl;
        for (vector<int> solution : liste_solution) {
            for(int i=0; i<n; i++){
                cout << solution.at(i) << " ";
            }
            cout << endl;
        }
    }

    // Verif des solution

    //version liste
    #pragma omp parallel
    {
        #pragma omp for private(fail_verif)
        for (vector<int> solution : liste_solution) {

            int* tab_verif = new int[taille_tab_verif](); // tableau qui permet de verifier dans la boucle si c'est correct

            //reset
            for(int j=0; j<taille_tab_verif; j++){
                tab_verif[j] = 0;
            }

            fail_verif = false;

            
            if(!solution.empty()){
                for (int j = 0; j < solution.size(); j++) {
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
            
            delete[] tab_verif;
        }

    }

    #ifndef _OPENMP
        stop = clock();
        total = (double)stop - start;
        CPU_time = total / CLOCKS_PER_SEC;
    #else
        stop_p = omp_get_wtime();
        CPU_time = stop_p - start_p;
    #endif

    printf("Time : %lf seconds\n", CPU_time);


    if(!liste_solution.empty()){
        cout << "liste non vide" << endl;
        for (vector<int> solution : liste_solution) {
            for(int i=0; i<n; i++){
                cout << solution.at(i) << " ";
            }
            cout << endl;
        }
    }


    cout << "Solutions non triviales >> " << cpt_no_trivaux << endl;
    cout << "Solutions >> " << cpt_solutions << endl;
    


    // delete[] tab_range;
    // delete[] tab_lock;
    // delete[] tab_verif;

    return EXIT_SUCCESS;
}//fin main