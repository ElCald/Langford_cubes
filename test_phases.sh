#!/bin/bash

# Nom de l'exécutable
EXECUTABLE="langford_v3"


# Vérification si l'exécutable existe
if [ -f $EXECUTABLE ]; then

    echo "Exécution lang 3..."
    ./$EXECUTABLE 3 > res3.txt

    echo "Exécution lang 4..."
    ./$EXECUTABLE 4 > res4.txt

    echo "Exécution lang 5..."
    ./$EXECUTABLE 5 > res5.txt

    echo "Exécution lang 6..."
    ./$EXECUTABLE 6 > res6.txt

else
    echo "Erreur : l'exécutable $EXECUTABLE n'existe pas."
fi