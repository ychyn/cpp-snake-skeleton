# Jeu du Snake en C++ 

Vous allez compléter un jeu classique de Snake en C++, directement jouable dans le terminal. Ce code ne ressemble pas à ce qu'on ferait dans la vraie vie mais il a le bon goût d'être simple à prendre en main. 

## Information pratique 

Pour commencer ce projet, vous devez **impérativement** forker le dépôt Git d'origine. Cela vous permettra de travailler sur votre propre copie, car vous n'avez pas les droits sur le dépôt d'origine. Il vous suffit : 

1. Rendez-vous sur le dépôt du projet sur Github (si vous lisez ceci c'est que vous y êtes !!).
2. Cliquez sur le bouton Fork en haut à droite pour créer une copie du projet sur votre propre compte GitHub.

## Structure du Projet

Le code est découpé en plusieurs parties que vous devrez compléter. Voici un aperçu des éléments principaux pour mieux comprendre comment le jeu fonctionne.

1. Le Background

Le terrain de jeu est représenté par une grille 2D de dimensions `nx x ny` (par défaut, 50x25).

Le background est stocké dans un tableau 1D (`std::vector<int>`), où chaque cellule de la grille correspond à une position donnée.
Signification des valeurs dans le tableau :

- 0 : Case vide.
- 1 : Bordure du terrain.
- 2 : Nourriture (que le serpent doit manger).
- 3 : un case du serpent (symboliser par `o`)

L'index dans le tableau 1D peut être calculé à partir des coordonnées 2D (x, y) comme suit :

```cpp
index = y * nx + x.
```
2. Le Serpent
Le serpent est représenté par un vecteur de paires de coordonnées (`std::vector<std::pair<int, int>>`), où chaque paire correspond à une case occupée par le serpent sur la grille.

La tête du serpent est le premier élément du vecteur. Le reste du vecteur correspond aux segments du corps, dans l'ordre.

Le serpent se déplace à l'aide des touches suivantes :

- z : Haut.
- q : Gauche.
- s : Bas.
- d : Droite.

Ces touches sont captées dans la fonction `snake_movement` que vous devez compléter pour convertir une touche en un vecteur directionnel :

## Ce que Vous Devez Faire

1. Compléter les fonctions vides
2. Compiler et Lancer le Jeu. Pour compiler le jeu, utilisez la commande suivante dans un terminal :

```bash
g++ -o snake snake.cpp
```

Pour lancer le jeu, exécutez simplement l'exécutable :

```bash
./snake
```

## Règles du Jeu

**Objectif** : Déplacez le serpent pour manger la nourriture (symbolisée par @).  
**Règle de croissance** : Chaque fois que le serpent mange une nourriture, il grandit d'une case.  
**Fin de partie** : Si le serpent entre en collision avec une bordure ou avec lui-même. Vous pouvez modifier la logique dans la fonction verifyBorder si vous le souhaitez.

## Bonus pour Aller Plus Loin
Pour les plus ambitieux, voici quelques idées de fonctionnalités supplémentaires à ajouter :

- Possibilité de passer à travers les murs : Si le serpent atteint une bordure, il réapparaît de l'autre côté.
- Augmentation de la vitesse : Réduire progressivement la valeur de lap pour rendre le jeu plus rapide.

## Aide et Conseils
Prenez le temps de bien comprendre le rôle de chaque fonction avant de commencer.

***Le code compile en l'état, mais le jeu n'est pas encore fonctionnel.***

Pour sortir du programme, vous pouvez utiliser la combinaison de touches `Ctrl + C`.


Testez chaque fonctionnalité progressivement, en imprimant des messages dans la console pour vérifier votre logique.
