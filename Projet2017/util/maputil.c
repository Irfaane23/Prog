#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#include "../include/map.h"
#include "../include/error.h"

int getValue(const char * msg){
	if (strcmp(msg, "solid") == 0){
		return MAP_OBJECT_SOLID;
	}
	if (strcmp(msg, "semi-solid") == 0){
		return MAP_OBJECT_SEMI_SOLID;
	}
	if (strcmp(msg, "air") == 0){
		return MAP_OBJECT_AIR;
	}
	if (strcmp(msg, "not-destructible") == 0){
		return 0;
	}
	if (strcmp(msg, "destructible") == 0){
		return MAP_OBJECT_DESTRUCTIBLE;
	}
	if (strcmp(msg, "not-collectible") == 0){
		return 0;
	}
	if (strcmp(msg, "collectible") == 0){
		return MAP_OBJECT_COLLECTIBLE;
	}
	if (strcmp(msg, "not-generator") == 0){
		return 0;
	}
	if (strcmp(msg, "generator") == 0){
		return MAP_OBJECT_GENERATOR;
	}


	return 1;
}

int main(int argc, char** argv){
	int file = open(argv[1],O_RDWR, 0666);
	int height, nbObject;
	int width;
	lseek(file,0,SEEK_SET);
	read(file,&width,sizeof(int));
	read(file,&height,sizeof(int));
	read(file,&nbObject,sizeof(int));

	if (strcmp(argv[2],"--setobjects") == 0) {
		char ** names = (char **)malloc(nbObject * sizeof(char *));
		int len;
		for (int i = 0; i < nbObject; i++) {
			read(file,&len, sizeof(int));
			names[i] = (char *)malloc(len * sizeof(char) + 1);
			for (int j = 0; j < len; j++) {
				read(file, &names[i][j], sizeof(char));
			}
			names[i][len] = '\0';
			lseek(file,5 * sizeof(int),SEEK_CUR);
		}

		int num = (argc - 3) / 6;
		int numFound = 0;
		int * res = (int *)malloc(num * sizeof(int));
		for (int i = 0; i < num; i++) {
			res[i] = 1;
			const char * name = argv[3 + 6 * i];
			for (int j = 0; j < nbObject; j++) {
				if (strcmp(name, names[j]) == 0) {
					res[i] = 0;
					numFound++;
					break;
				}
			}
		}

		int fileTMP = open("tmp.map",O_CREAT|O_RDWR, 0666);
		int newNbObject = nbObject + num - numFound;
		write(fileTMP,&width,sizeof(int));
		write(fileTMP,&height,sizeof(int));
		write(fileTMP,&newNbObject,sizeof(int));

		for (int i = 0; i < num; i++) {
			if (res[i] == 0) {
				continue;
			}
			const char * name = argv[3 + 6 * i];
			int taille = strlen(name);
			write(fileTMP,&taille,sizeof(int));
			write(fileTMP,name,taille);
			for (int j = 0; j < 5; j++){
				int value = getValue(argv[4 + j + i * 6]);
				write(fileTMP,&value, sizeof(int));
			}
		}

		lseek(file,3 * sizeof(int),SEEK_SET);
		char ch;
		while(read(file,&ch,sizeof(char)) != 0){
			write(fileTMP, &ch, sizeof(char));
		}
		char order[50] = {'\0'};
		strcat(order, "mv tmp.map ");
		strcat(order, argv[1]);
		system(order);
		free(res);
		for (int i = 0; i < nbObject; i++) {
			free(names[i]);
		}
		return 0;
	}

	if (strcmp(argv[2],"--getwidth") == 0) {
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
