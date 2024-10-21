/**
 * Algorithme de Langford sans cas triviaux et avec symétries
 * @author Eliot CALD
 * @version 3.4
 */

#include <iostream>
#include <stack>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <climits>

#include "trachtenberg.h"

using namespace std;

#define NB_PARAM 1


/**
 * Elimine les cas trivaux, donc si on retrouve 2 fois la même valeur dans le tableau
 * A voir pour retirer les valeurs qui sont adjacente et qui ont juste une case d'écart
 * @param tab Tableau tab_range
 * @param taille Taille du tableau
 * @return True s'il y a un cas triviale et false dans le cas où c'est conforme
 */
inline bool cas_triviaux(int* tab, int taille){

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

    int n = atoi(argv[1]); // Valeur N de langford
    unsigned long long k = 1; // nb de solutions possible avec cas triviaux
    int val = 0; // valeur à placer dans la case
    int taille_tab_verif = n*2; // Taille du tableau de vérif qui est de taille 2N, la tableau où on place les croix
    bool fail_verif = false; // Variable qui est true si la solution testée est correcte
    unsigned long long cpt_solutions = 0; // compteur du nombre de solutions finales
    unsigned long long cpt_no_trivaux = 0; // compteur de solutions non triviales
    int n_val = n; // variable qui permet le calcul de la valeur du modulo qui est décémenté
    char* solution_total = "1"; //variable contient le nb total de solution
    char sol_temp[10] = "";
    int k_temp = 1;


    // Variable pour calculer le temps d'execution
    clock_t start, stop, mid_time;                          // seq
    double start_p, stop_p, mid_time_p, CPU_time, total;    // omp



    #ifndef _OPENMP
        cout << "Execution séquentielle..." << endl;
        start = clock();
    #else
        cout << "Execution parallèle..." << endl;
        start_p = omp_get_wtime();
    #endif

    

    // calcul du nb de solutions totale
    for(int i=0; i<n; i++){ 
        if(n<10){
            k *= ((n-1) + i);
        }
        k_temp = ((n-1) + i);
        sprintf(sol_temp, "%d", k_temp);
        solution_total = trachtenberg(sol_temp, solution_total);
    }

    cout << "Vraie valeur : " << solution_total << endl;


    cout << "Solutions totales >> " << k << endl;

    
    #pragma omp parallel private(n_val, fail_verif) 
    {

        int* tab_range = new int[n](); // tableau de solutions individuelle
        int* tab_lock = new int[n](); // tableau pour savoir si la case peut avancer ou pas
        int* tab_verif = new int[taille_tab_verif](); // tableau qui permet de verifier dans la boucle si c'est correct
    
        #ifdef _OPENMP
            #pragma omp single
            cout << "Nb threads : " << omp_get_num_threads() << endl;
        #endif


        #pragma omp for
        for(int i=0; i<n-1; i++){
            
            tab_range[n-1] = i; // initialisation de la dernière case pour permettre au thread de se partager le travail
            
            while(tab_range[n-1] == i){ // chaque thread avance tant qu'ils génèrent des solutions qui on comme dernière case la valeur qui leur est attribuée
                
                #pragma critical
                {
                    if(cas_triviaux(tab_range, n) == false){ // si c'est pas un cas triviale on le passe au test de langford
                        cpt_no_trivaux++;

                        for(int j=0; j<taille_tab_verif; j++){
                            tab_verif[j] = 0;
                        }


                        fail_verif = false;

                        // vérif langford
                        for (int j = 0; j < n; j++) {
                            if(tab_verif[tab_range[j]] == 0 && tab_verif[tab_range[j]+j+2] == 0){
                                tab_verif[tab_range[j]] = j+1;
                                tab_verif[tab_range[j]+j+2] = j+1;
                            }
                            else{
                                fail_verif = true;
                                break;
                            }
                        }
                        

                        if(!fail_verif){
                            cpt_solutions++;
                        }
                        
                        

                    }                    
                }


                //génération de la solution suivante

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

        delete[] tab_range;
        delete[] tab_lock;
        delete[] tab_verif;
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


    cout << "Solutions non triviales >> " << cpt_no_trivaux << endl;
    cout << "Solutions >> " << cpt_solutions << endl;



    return EXIT_SUCCESS;
}//fin main