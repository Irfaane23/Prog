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

  // Ajout des fleurs
  map_object_add("images/flower.png",1,MAP_OBJECT_AIR);
  // Ajout  des pièces
  map_object_add("images/coin.png",20,MAP_OBJECT_AIR || MAP_OBJECT_COLLECTIBLE);

  map_object_end ();
}

void map_save (char *filename)
{
   // fprintf (stderr, "Sorry: Map save is not yet implemented\n");

  // TODO
  if(filename == NULL){
    fprintf (stderr, "Sorry: Map save is not yet implemented\n");

  }
//  filename = "sauvegarde.txt";
  int fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC,0666);
  int width =  map_width();
  int height = map_height();
  int nbObjet = map_objects();

  char buffer[10];


  sprintf(buffer,"%d\n",width);
  write(fd,&buffer,sizeof(buffer));
  write(fd,"!",sizeof(char));
  sprintf(buffer,"%d\n",height);
  write(fd,&buffer,sizeof(buffer));
  write(fd,"!",sizeof(char));

  sprintf(buffer,"%d\n",nbObjet);
  write(fd,&buffer,sizeof(buffer));
  write(fd,"!",sizeof(char));

  int contenuCase;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      contenuCase = map_get(i,j);

      if(contenuCase != MAP_OBJECT_NONE){
        char *fichierPNG = map_get_name(contenuCase);
        int frames = map_get_frames(contenuCase);
        int solidity = map_get_solidity(contenuCase);

        int isDestructible = map_is_destructible(contenuCase);
        int isCollectible = map_is_collectible(contenuCase);
        int isGenerator = map_is_generator(contenuCase);
        write(fd,"!",sizeof(char));//début élement
        write(fd,&fichierPNG,sizeof(strlen(fichierPNG)));
        write(fd,"!",sizeof(char));//fin du nom de l'élement

        sprintf(buffer,"%d",frames);
        write(fd,&buffer,sizeof(buffer));
        write(fd,"!",sizeof(char));//fin de la taille du frame

        sprintf(buffer,"%d",solidity);
        write(fd,&buffer,sizeof(buffer));

        sprintf(buffer,"%d",isDestructible);
        write(fd,&buffer,sizeof(buffer));

        sprintf(buffer,"%d",isCollectible);
        write(fd,&buffer,sizeof(buffer));

        sprintf(buffer,"%d",isGenerator);
        write(fd,&buffer,sizeof(buffer));

      }
      else
        write(fd,"0",sizeof(char));
    }
  }
  close(fd);
  printf("save done i guess");

}

void map_load (char *filename)
{
   // TODO

   int width,height,nbObject,frames,solidity,isDestructible,isCollectible,isGenerator;
   char *fichierPNG;
   int file=open(filename,O_RDONLY,0666);
   char *tmp;
   char *word;

   int pos_x,pos_y;




   //sequence à répéter pour chaque données à taille variable du fichier de sauvegarde, peut être simplifier par une fonction int f(int fd) qui lit jusqu'au prochain '!'
   //width
   read(file,tmp,sizeof(char));
   while(strcmp(tmp,"!")){
      strcat(word,tmp);
//      str_append(word,tmp);
      read(file,tmp,sizeof(char));
   }
   width=atoi(word);
   word="";
   //height
   read(file,tmp,sizeof(char));
   while(strcmp(tmp,"!")){
      strcat(word,tmp);
      read(file,tmp,sizeof(char));
   }
   height=atoi(word);
   word="";
   //nbObject
    read(file,tmp,sizeof(char));
    while(strcmp(tmp,"!")){
      strcat(word,tmp);
      read(file,tmp,sizeof(char));
   }
   nbObject=atoi(word);
   word="";

//Création de la map, ajout des élements
   map_allocate(width,height);
   char *tabObject[nbObject]; //permettra d'étudier si un object a déjà été add dans la map
   int nbElmnt=0;
   map_object_begin(nbObject);

   //début lecture matrice terrain
   for(int i = 0;i <width; i++){
      for(int j = 0; j < height ; j++){
         if(strcmp(tmp,"!")){
            //début read des éléments
            //je le ferai plus tard wallah
            while(strcmp(tmp,"!")){
               strcat(word,tmp);
               read(file,tmp,sizeof(char));
            }
            fichierPNG=word;
            word="";
            while(strcmp(tmp,"!")){
               strcat(word,tmp);
               read(file,tmp,sizeof(char));
             }
            frames=atoi(word);
            word="";
            read(file,word,sizeof(char));
            solidity=atoi(word);
            read(file,word,sizeof(char));
            isDestructible=atoi(word);
            read(file,word,sizeof(char));
            isCollectible=atoi(word);
            read(file,word,sizeof(char));
            isGenerator=atoi(word);
            //fin lecture des informations de l'élément
            read(file,tmp,(sizeof(char)));//passage au prochain caractère
            pos_x=i;
            pos_y=j;
            int k=0;
            while(k<nbObject && (strcmp(fichierPNG,tabObject[k]))!=0){ //on vérifie que l'élement n'a pas déjà été ajouté
               k++;
            }
            if(k==nbObject){ //s'il n'a jamais été ajouté on fait map_object _add et on l'ajoute sur la map
               map_object_add(fichierPNG,frames,solidity);//doit mettre en avant isDestructible etc....
               tabObject[nbElmnt]=fichierPNG;
               nbElmnt++;
               map_set(pos_x,pos_y,nbElmnt-1);
            }
            else{ //sinon on ne fait que l'ajouter sur la map
               map_set(pos_x,pos_y,k);
            }

         }
         else{//on a un 0 donc pas un élément
            read(file,tmp,sizeof(char));
            //on zappe et on passe à la suite maggle
         }
      }
   }
   //fin de lecture de tous les éléments.
   map_object_end();
   close(file);
   //it_with_error ("Map load is not yet implemented\n");
}
#endif
