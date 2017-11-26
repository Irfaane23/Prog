
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>


int main(int argc, char** argv){
  int file = open(argv[1],O_RDWR, 0666);
  int height, nbObject;
  int width ;
  lseek(file,0,SEEK_SET);
  read(file,&width,sizeof(int));
  read(file,&height,sizeof(int));
  read(file,&nbObject,sizeof(int));
  
  if (strcmp(argv[2],"--setobjects") == 0) {
		int fileTMP = open("tmp.map",O_CREAT|O_RDWR, 0666);
		int num = (argc - 1) % 6;
		nbObject += num;
		write(fileTMP,&width,sizeof(int));
		write(fileTMP,&height,sizeof(int));
		write(fileTMP,&nbObject,sizeof(int));
		for (int i = 0; i < num; i++) {
			const char * name = argv[3 + 6 * i];
			int taille = strlen(name);
			printf("\n\n\n\n%d\n\n\n\n", taille);
			write(fileTMP,&taille,sizeof(int));
			write(fileTMP,name,taille);
			for (int j = 0; j < 5; j++){
				int value = getValue(argv[4 + j + i * 6]);
				write(fileTMP,&value, sizeof(int));
			}
		}
		char ch;
		while(read(file,&ch,sizeof(char)) != 0){
			write(fileTMP, &ch, sizeof(char));
		}
		system("rm saved.map");
		system("mv tmp.map saved.map");
		return 0;
	}

  else if (strcmp(argv[2],"--getwidth") == 0) {
    // on lit la valeur de map_width
     // write(1, &width, nbRead);
   // printf("test  : %d\n", nbRead);
     printf("width  : %d\n", width);
  }

  else if (strcmp(argv[2],"--getheight") == 0) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
       
    printf("height  : %d\n", height);
  }
  else if (strcmp(argv[2], "--getobject") == 0) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    printf("nbObject :  %d\n", nbObject);
  }
  else if (strcmp(argv[2], "--getinfo") == 0) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    printf("width  : %d\n", width);
    printf("height  : %d\n", height);
    printf("nbObjet  : %d\n", nbObject);
  }

  else if (strcmp(argv[2],"--setwidth") == 0) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    width = atoi(argv[3]);
    lseek(file,0, SEEK_SET);
    write(file,&width, sizeof(int));
    printf("width  : %d\n", width);
  }
  else if (strcmp(argv[2],"--setheight") == 0 ) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
    height = atoi(argv[3]);
    lseek(file,sizeof(int), SEEK_SET);
    write(file,&height, sizeof(int));
    printf("height  : %d\n", height);
  }
  else if (strcmp(argv[2], "--setwidth") == 0 && strcmp(argv[4],"--setheight") == 0) {
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
  else if (strcmp(argv[4], "--setwidth") == 0 && strcmp(argv[2], "--setheight") == 0) {
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
