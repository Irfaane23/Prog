#include "SDL.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>


int main(int argc, char** argv){
  int file = open(argv[1], ,O_RDONLY | O_WRONLY | O_TRUNC, 0666);
  int width, height, nbObject;

  read(file,&width,sizeof(int));
  read(file,&height,sizeof(int));
  read(file,&nbObject,sizeof(int));

  if (strcmp(argv[2], "--getwidth")) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    printf("width  : %d\n", width);
  }

  if (strcmp(argv[2], "--getheight")) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    //read(file, &height,sizeof(int));
    printf("height  : %d\n", height);
  }
  if (strcmp(argv[2], "--getobject")) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    printf("nbObject :  %d\n", nbObject);
  }
  if (strcmp(argv[2], "--getinfo")) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    printf("width  : %d\n", width);
    printf("height  : %d\n", height);
    printf("nbObjet  : %d\n", nbObject);
  }

  if (strcmp(argv[2], "--setwidth") && argc == 3) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    width = atoi(argv[3]);
    lseek(file,0, SEEK_SET);
    write(file,&width, sizeof(int));
    printf("width  : %d\n", width);
  }
  else if (strcmp(argv[2], "--setheigth") && argc == 3 ) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    height = atoi(argv[3]);
    lseek(file,sizeof(int), SEEK_SET);
    write(file,&height, sizeof(int));
    printf("height  : %d\n", height);
  }

  else if (strcmp(argv[2], "--setwidth") && strcmp(argv[4], "--setheight")) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    width = atoi(argv[3]);
    lseek(file,0, SEEK_SET);
    write(file,&width, sizeof(int));

    height = atoi(argv[5]);
    lseek(file,sizeof(int), SEEK_SET);
    write(file,&height, sizeof(int));
    printf("width  : %d\n", width);
    printf("height  : %d\n", height);

  }

  else if (strcmp(argv[4], "--setwidth") && strcmp(argv[2], "--setheight")) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    width = atoi(argv[5]);
    lseek(file,0, SEEK_SET);
    write(file,&width, sizeof(int));

    height = atoi(argv[3]);
    lseek(file,sizeof(int), SEEK_SET);
    write(file,&height, sizeof(int));

    printf("width  : %d\n", width);
    printf("height  : %d\n", height);

  }

  else{
    fprintf(stderr, "./maputil <argument> <int>\n", );
  }

}
