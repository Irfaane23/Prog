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

//*
int main(int argc, char** argv){
	int file = open(argv[1],O_RDWR, 0666);
	int height, nbObject;
	int width;
	lseek(file,0,SEEK_SET);
	read(file,&width,sizeof(int));
	read(file,&height,sizeof(int));
	read(file,&nbObject,sizeof(int));
	if (strcmp(argv[2],"--pruneobjects") == 0) {
		int len;
		// go to the map zone
		for (int i = 0; i < nbObject; i++) {
			read(file,&len, sizeof(int));
			lseek(file,len * sizeof(char),SEEK_CUR);
			lseek(file,5 * sizeof(int),SEEK_CUR);
		}
		int value;
		// check which objects exist in the map
		// if ith object exists, res[i] will greater than 0
		int *res = (int *)malloc(nbObject * sizeof(int));
		memset(res, 0, nbObject * sizeof(int));
		while(read(file,&value,sizeof(int)) != 0){
			if (value < 0) {
				continue;
			}
			res[value] = 1;
		}
		int num = 0; // count how many objects do not exist in the map
		for (int i = 0; i < nbObject; i++){
			if (res[i] == 0){
				num++;
			}
		}
		int fileTMP = open("tmp.map",O_CREAT|O_RDWR, 0666);
		int newNbObject = nbObject - num;
		write(fileTMP,&width,sizeof(int));
		write(fileTMP,&height,sizeof(int));
		write(fileTMP,&newNbObject,sizeof(int));
		char ch;
		// filter the objects in the current saved.map
		lseek(file,3 * sizeof(int),SEEK_SET);
		for (int i = 0; i < nbObject; i++){
			if (res[i] != 0) {
				read(file,&len, sizeof(int));
				write(fileTMP, &len, sizeof(int));
				for (int j = 0; j < len; j++){
					read(file,&ch,sizeof(char));
					write(fileTMP, &ch, sizeof(char));
				}
				for (int j = 0; j < 5; j++){
					read(file,&value,sizeof(int));
					write(fileTMP, &value, sizeof(int));
				}
			}
			else {
				// don't add it to the new saved.map if not exists
				read(file,&len, sizeof(int));
				lseek(file,len * sizeof(char),SEEK_CUR);
				lseek(file,5 * sizeof(int),SEEK_CUR);
			}
		}
		/*
		 * reorganize the values of the map zone
		 * remove the gaps of the values
		 * for example,
		 * if the numbers of the existing objects are:
		 * 1, 3, 7
		 * which means that
		 * res[0] = 0 
		 * res[1] = 11 (res[1] shows 11 times)
		 * res[2] = 0
		 * res[3] > 34
		 * ...
		 * res[7] = 5
		 * ...
		 * now we need to make them as below:
		 * 0, 1, 2
		 * which means that
		 * res[0] = 0
		 * res[1] = 0 (this 0 means 0th object, there is no conflict with with 0 meaning "not exist")
		 * res[2] = 0
		 * res[3] = 1
		 * ...
		 * res[7] = 2
		 * ...
		 */
		int counter = 0;
		for (int i = 0; i < nbObject; i++) {
			if (res[i] != 0) {
				res[i] = counter++;
			}
		}
		int val;
		while(read(file,&val,sizeof(int)) != 0){
			if (val == -1) {
				write(fileTMP, &val, sizeof(int));
			}
			else {
				write(fileTMP, &res[val], sizeof(int));
			}
		}
		char order[50] = {'\0'};
		strcat(order, "mv tmp.map ");
		strcat(order, argv[1]);
		system(order);
		free(res);
		return 0;
	}




	if (strcmp(argv[2],"--setobjects") == 0) {
		// names is to save the existed objects in the saved.map
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
		/*
		 * if res[i] is 1, it means that the ith object from argv
		 * does not exist whereas 0 means it exists
		 * if the object from argv existed, it shouldn't be added
		 * into the saved.map
		 */
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

		// fileTMP is a temporary file, which will be the new saved.map
		int fileTMP = open("tmp.map",O_CREAT|O_RDWR, 0666);
		int newNbObject = nbObject + num - numFound;
		write(fileTMP,&width,sizeof(int));
		write(fileTMP,&height,sizeof(int));
		write(fileTMP,&newNbObject,sizeof(int));

		// add the existed objects into fileTMP
		lseek(file,3 * sizeof(int),SEEK_SET);
		int value;
		char ch;
		for (int i = 0; i < nbObject; i++) {
			read(file,&len, sizeof(int));
			write(fileTMP, &len, sizeof(int));
			for (int j = 0; j < len; j++){
				read(file,&ch,sizeof(char));
				write(fileTMP, &ch, sizeof(char));
			}
			for (int j = 0; j < 5; j++){
				read(file,&value,sizeof(int));
				write(fileTMP, &value, sizeof(int));
			}
		}

		// add the new objects from argv
		for (int i = 0; i < num; i++) {
			if (res[i] == 0) {
				continue;
			}
			const char * name = argv[3 + 6 * i];
			int taille = strlen(name);
			write(fileTMP,&taille,sizeof(int));
			write(fileTMP,name,taille);
			for (int j = 0; j < 5; j++){
				value = getValue(argv[4 + j + i * 6]);
				write(fileTMP,&value, sizeof(int));
			}
		}

		// add the rest of file into fileTMP
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
