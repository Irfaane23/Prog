#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "error.h"

#ifdef PADAWAN

void map_new (unsigned width, unsigned height)
{
  map_allocate (width, height);

  for (int x = 0; x < width; x++)
    map_set (x, height - 1, 0); // Ground

  for (int y = 0; y < height - 1; y++) {
    map_set (0, y, 1); // Wall
    map_set (width - 1, y, 1); // Wall
  }

  map_object_begin (8);

  // Texture pour le sol
  map_object_add ("images/ground.png", 1, MAP_OBJECT_SOLID);
  // Mur
  map_object_add ("images/wall.png", 1, MAP_OBJECT_SOLID);
  // Gazon
  map_object_add ("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
  // Marbre
  map_object_add ("images/marble.png", 1, MAP_OBJECT_SOLID | MAP_OBJECT_DESTRUCTIBLE);
  // Herbe
  map_object_add ("images/herb.png", 1, MAP_OBJECT_AIR);
  // Petit plancher flottant
  map_object_add ("images/floor.png", 1, MAP_OBJECT_SEMI_SOLID);

  // Fleur pour égayer le paysage
  map_object_add("images/flower.png", 1, MAP_OBJECT_AIR);
  //Pièces à collecter
  map_object_add("images/coin.png",20, MAP_OBJECT_AIR | MAP_OBJECT_COLLECTIBLE);

  map_object_end ();
}

void map_save (char *filename)
{
  int file = open(filename,O_CREAT | O_WRONLY|O_TRUNC , 0666);


  //On recupere la largeur de la carte puis la hauteur
  int width = map_width();
  int height = map_height();
  int nbObjet = map_objects(); // on recupere le nombre d'objets differents
  //On écrit dans le fichier la largeur, la hauteur et le nombre d'objets differents
  write(file,&width,sizeof(int));
  write(file,&height,sizeof(int));
  write(file,&nbObjet,sizeof(int));

  int frame, solidity, isDestructible, isGenerator, isCollectible;
  char* fichierPNG;
  int taille;

 //on envoie tous les objets différents de la map
  for (int i = 0; i < nbObjet; i++) {
    fichierPNG = map_get_name(i);
    taille = strlen(fichierPNG);
    //on envoie dans le fichier la longueur du nom de l'objet
    write(file,&taille,sizeof(int));
    //tant qu'on arrive pas à la fin du nom de l'objet, on envoie caractère par caractère dans le file
    for(int indice=0;indice < taille;indice++) {
      write(file,&fichierPNG[indice], sizeof(char));
    }
    /* On ecrit dans le fichier le nom,le nombre de sprites du fichier ainsi que les
      les propriétés qu'il peut avoir
    */
    frame = map_get_frames(i);
    write(file,&frame,sizeof(int));


    solidity = map_get_solidity(i);
    write(file,&solidity,sizeof(int));


    isDestructible = map_is_destructible(i);
    write(file,&isDestructible,sizeof(int));


    isCollectible = map_is_collectible(i);
    write(file,&isCollectible,sizeof(int));


    isGenerator = map_is_generator(i);
    write(file,&isGenerator,sizeof(int));

  }

  int contenuCase;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      contenuCase = map_get(i,j); //numéro de l'objet dans la liste des objets de la map
      write(file,&contenuCase,sizeof(int));
    }
  }

  close(file);
  fprintf (stderr, "Done: Map save is implemented\n");
}

void map_load (char *filename)
{
  int file = open(filename,O_RDONLY);
  int height, width, nbObject;

  //Lecture de la hauteur et de la largeur
  read(file,&width,sizeof(int));
  read(file,&height,sizeof(int));

  map_allocate(width,height);

  read(file,&nbObject,sizeof(int));
  map_object_begin(nbObject); //début initialisation de la liste des objets de la map

  char* fichierPNG=NULL;
  int taille=-1 , nbSprite, properties;
  int buffer; //va contenir toutes les informations récupérés lors du read.
              //Il représentera les propriétés des objets de la map
              //mais aussi le contenu des éléments de la matrice qui permettra d'écrire
              //sur la map via map_set(x,y,buffer)

  for (int i = 0; i < nbObject; i++) {
    read(file,&buffer,sizeof(int)); //compte le nombre de caractère dans le nom du des objets
    if (buffer> taille) {
      fichierPNG = realloc(fichierPNG, buffer*sizeof(char));
      taille = buffer;
    }
    for (int j = 0; j < buffer; j++) {
      read(file,&fichierPNG[j], sizeof(char));
    }

    fichierPNG[buffer] = '\0';//fin du nom du fichierPNG
    read(file,&nbSprite,sizeof(int));
    read(file,&properties,sizeof(int));
    read(file,&buffer,sizeof(int));

    //cas sur les proprietes de l'objet : MAP_OBJECT_COLLECTIBLE, MAP_OBJECT_COLLECTIBLE, MAP_OBJECT_GENERATOR
    if (buffer) {
      properties =  properties | MAP_OBJECT_DESTRUCTIBLE;
    }
    read(file,&buffer,sizeof(int));
    if (buffer) {
      properties =  properties | MAP_OBJECT_COLLECTIBLE;
    }
    read(file,&buffer,sizeof(int));

    if (buffer) {
      properties =  properties | MAP_OBJECT_GENERATOR;
    }

    //une fois toutes les variables récupérées via read, ajout de l'objet à la liste d'objet de la map
    map_object_add(fichierPNG,nbSprite,properties);

  }
  map_object_end(); //fin création de la liste d'objet
  free(fichierPNG); //libération du malloc utilisé pour le nom du fichier
  //écriture sur la matrice de la map via map_set.
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      read(file,&buffer,sizeof(int));
      if (buffer != MAP_OBJECT_NONE) {
        map_set(i,j,buffer);
      }
    }
  }
  close(file);
}

#endif
