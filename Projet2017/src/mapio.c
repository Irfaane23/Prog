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

  map_object_begin (6);

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

  // Ajout des fleurs
  map_object_add("images/flower.png",1,MAP_OBJECT_AIR);
  // Ajout  des pièces
  map_object_add("images/coin.png",20,MAP_OBJECT_AIR && MAP_OBJECT_COLLECTIBLE);

  map_object_end ();
}

void map_save (char *filename)
{

  //fprintf (stderr, "Sorry: Map save is not yet implemented\n");

  // TODO
  if(filename == NULL){
    fprintf (stderr, "Sorry: Map save is not yet implemented\n");

  }
  //filename = "sauvegarde.txt";
  int fd = open(filename,O_WRONLY |O_CREAT|O_TRUNC,0666);
  int width = map_width();
  int height = map_height();
  int nbObjet = map_objects();


  write(filename,&width,sizeof(char));
  write(filename,&height,sizeof(char));
  write(filename,&nbObjet,sizeof(char));

  int contenuCase;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      contenuCase = map_get(i,j);

      if(contenuCase != MAP_OBJECT_NONE){
        char* fichierPNG = map_get_name(contenuCase);
        int frames = map_get_frames(contenuCase);
        int solidity = map_get_solidity(contenuCase);

        int isDestructible = map_is_destructible(contenuCase);
        int isCollectible = map_is_collecible(contenuCase);
        int isGenerator = map_is_generator(contenuCase);
        //créer un signal pour que le loader sache qu'il y a un contenu
        write(filename,strlen(fichierPNG),sizeof(fichierPNG));
        for (int l = 0; l < strlen(fichierPNG); l++) {
          /* code */
          write(filename, fichierPNG[i], sizeof(char));
        }
        write(filename,&frames,sizeof(char));
        write(filename,&solidity,sizeof(char));
        write(filename,&isDestructible,sizeof(char));
        write(filename,&isCollectible,sizeof(char));
        write(filename,&isGenerator,sizeof(char));

      }
      else{
        //write(filename,'0',sizeof(char)); vérifier que '0' passe en char
      }
    }
  }
  close(fd);
}

void map_load (char *filename)
{
  // TODO
  exit_with_error ("Map load is not yet implemented\n");
}

#endif