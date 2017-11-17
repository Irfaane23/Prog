
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>


int main(int argc, char** argv){
  int file = open(argv[1],O_RDONLY | O_WRONLY | O_TRUNC, 0666);
  int width, height, nbObject;

  read(file,&width,sizeof(int));
  read(file,&height,sizeof(int));
  read(file,&nbObject,sizeof(int));

  if (strcmp(argv[2],"--getwidth")==0) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    printf("width  : %d\n", width);
  }

  if (strcmp(argv[2] =="--getheight") == 0) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    //read(file, &height,sizeof(int));
    printf("height  : %d\n", height);
  }
  if (strcmp(argv[2]== "--getobject") == 0) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    printf("nbObject :  %d\n", nbObject);
  }
  if (strcmp(argv[2]== "--getinfo") == 0) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    printf("width  : %d\n", width);
    printf("height  : %d\n", height);
    printf("nbObjet  : %d\n", nbObject);
  }

  if (strcmp(argv[2]=="--setwidth") == 0 && argc == 3) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    width = atoi(argv[3]);
    lseek(file,0, SEEK_SET);
    write(file,&width, sizeof(int));
    printf("width  : %d\n", width);
  }
  if (strcmp(argv[2]=="--setheigth") == 0 && argc == 3 ) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    height = atoi(argv[3]);
    lseek(file,sizeof(int), SEEK_SET);
    write(file,&height, sizeof(int));
    printf("height  : %d\n", height);
  }

  if (strcmp(argv[2]== "--setwidth") == 0 && strcmp(argv[4]=="--setheight") == 0) {
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

  if (strcmp(argv[4]== "--setwidth") == 0 && strcmp(argv[2]== "--setheight") == 0) {
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
    fprintf(stderr, "./maputil <argument> <int>\n");
  }

}
