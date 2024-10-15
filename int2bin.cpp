#include <iostream>
#include <vector>
#include <algorithm>


#define N 3

// converti un nombre int en sa base N (ex: en binaire, on hexa, etc)

using namespace std;

void convertirEnBinaire(int nombre) {
    // Vector pour stocker les bits
    vector<int> bits;

    // Si le nombre est 0, afficher directement 0
    if (nombre == 0) {
        cout << "Représentation binaire : 0" << endl;
        return;
    }

    // Convertir en binaire
    while (nombre > 0) {
        bits.push_back(nombre % N); // Ajouter le bit le moins significatif
        nombre /= N; // Diviser par 2
    }

    // Afficher le binaire en inversant le vecteur (car on l'a construit à l'envers)
    cout << "Représentation binaire : ";
    reverse(bits.begin(), bits.end()); // Inverser le vecteur
    for (int bit : bits) {
        cout << bit;
    }
    cout << endl;
}

int main() {
    int nombre;

    // Demander un entier à l'utilisateur
    cout << "Entrez un entier : ";
    cin >> nombre;

    // Convertir et afficher en binaire
    convertirEnBinaire(nombre);

    return 0;
}
