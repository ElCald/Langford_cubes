/**
 * Algorithme de Langford
 * @author Elior CALD
 * @version 1.2
 */

#include <iostream>
#include <stack>


#define NB_PARAM    2

using namespace std;

int* tab; //tableau de langford
int* tab_value; //tableau circulaire des valeurs à entrer

void afficher_tab(int t){
    for(int k=0; k<t; k++){
        cout << tab[k] << " ";
    }
    cout << endl;
}

int main(int argc, char* argv[]){

    if(argc != NB_PARAM +1){
        cerr << "2 paramètres : nb d'éléments et leur répétition, ex : l(4,2)\n" << endl;
        exit(EXIT_FAILURE);
    }

    int nb_valeur = atoi(argv[1]);              //nombre de valeur à placer
    int nb_repetition = atoi(argv[2]);          //nombre de fois qu'on doit répéter chaque valeurs
    int taille_tab = nb_valeur*nb_repetition;   //taille du tableau = nb de valeurs x nb de répétition
    int nb_elemt = 0;                           //nb de valeurs dans le tableau de langford
    int n = 0;                                  //valeur à placer dans le tableau de langford
    int i = 0;                                  //indice du placemnt de la valeur dans le tableau
    int i_value = 0;                            //indice du tableau de valeur
    int stop = 0;                               //condition d'arrêt de la boucle de test
    int stop_solution = 0;                      //condition d'arrêt de la boucle de solution
    int stop_main = 0;                          //condition d'arrêt de la boucle principale
    int place_dispo = 0;                        //condition pour si on ne trouve pas de place
    

    tab = (int*)malloc(sizeof(int) * taille_tab);
    tab_value = (int*)malloc(sizeof(int) * nb_valeur);

    stack<int> i_last; //pile des derniers indices placés


    // Init tab circulaire valeurs dans l'ordre décroissant (ex: nb_valeur=4 et tab_value = {4,3,2,1})
    for(int k=0; k<nb_valeur; k++){
        tab_value[k] = nb_valeur-k;
    }


    // Init tab langford à zéro
    for(int k=0; k<taille_tab; k++){
        tab[k] = 0;
    }

    
    n = tab_value[0];
    i_value = 0;
    nb_elemt = 0;

    stop_main = 0;

    // boucle principale qui ne s'arrête que lorsque la grande valeur atteint la fin du tableau 
    while(stop_main != 1 && tab[taille_tab-1] != nb_valeur){

        stop_solution = 0;
        while(stop_solution != 1 && nb_elemt < nb_valeur){ // boucle pour chercher une solution
            place_dispo = 1;
            stop = 0;
            while(stop != 1 && i<taille_tab){ // boucle de test dans le tableau pour le placement

                if(i+n+1 < taille_tab){ // Evite de sortir du tableau avec la 2eme valeur à placer

                    if(tab[i] == 0 && tab[i+n+1] == 0){ // Vérification dans le tableau
                        tab[i] = n;
                        tab[i+n+1] = n;

                        i_last.push(i);

                        nb_elemt++;
                        i_value = (i_value+1)%nb_valeur; // on passe à la valeur suivante à placer
                        n = tab_value[i_value];

                        stop = 1;
                        i = 0;
                    }
                    else{ // Si pas de place on passe à la case d'après
                        i++;
                    }
                }
                else{ // Si la 2ème valeur à placer sort du tableau, on arrête de le parcourir
                    stop = 1;
                    place_dispo = 0;
                }
        
            }

            if(!i_last.empty()){ // Evite de taper dans la pile si elle est vide (en gros on évite le seg fault)
                if(place_dispo == 0){ // on n'a jamais trouvé de place donc on prend le dernier élément placé et on le décale d'une case

                    i = i_last.top();
                    i_last.pop();

                    nb_elemt--;

                    i_value = (i_value-1)%nb_valeur;

                    if(i_value < 0){
                        i_value += nb_valeur;
                    }

                    n = tab_value[i_value];


                    tab[i] = 0;
                    tab[i+n+1] = 0;

                    i++;
                }
            }
            else{
                stop_solution = 1;
            }
        }
        
        afficher_tab(taille_tab);

        if(!i_last.empty()){ // Evite de taper dans la pile si elle est vide (en gros on évite le seg fault)
            if(tab[taille_tab-1] != nb_valeur){ // Si on n'a pas trouvé toutes les solutions pour une valeur alors on relance comme si la solution trouvé n'était pas correcte

                i = i_last.top();
                i_last.pop();

                nb_elemt--;

                i_value = (i_value-1)%nb_valeur;

                if(i_value < 0){
                    i_value += nb_valeur;
                }

                n = tab_value[i_value];


                tab[i] = 0;
                tab[i+n+1] = 0;

                i++;
            }
        }
        else{
            stop_main = 1;
        }

    }

    free(tab);
    free(tab_value);

    return EXIT_SUCCESS;
}