
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

int getValue(const char * msg){
	if (strcmp(msg, "not-destructible") == 0){
		return 0;
	}
	if (strcmp(msg, "not-collectible") == 0){
		return 0;
	}
	if (strcmp(msg, "not-generator") == 0){
		return 0;
	}
	// solid missing
	return 1;
}
typedef struct element{
	int x;
	int y;
	int value;
}element;

void setmap(int file,int oldWidth,int oldHeight,int newWidth, int newHeight){
	//récupérer tous les objets de la matrice et leurs coordonnées
	printf("\nentrée dans set map\n");
	lseek(file,0,SEEK_END-(oldWidth*oldHeight));
	int contenu;
	element *tabElmnt=(element*)malloc((oldWidth*oldHeight)*sizeof(element));
	int nbelmnt=0;
	for(int i=0;i<oldWidth;i++){
		for(int j=0;j<oldHeight;j++){
			read(file,&contenu,sizeof(int));
			if(contenu!=0){
				tabElmnt[nbelmnt].x=i;
				tabElmnt[nbelmnt].y=j;
				tabElmnt[nbelmnt].value=contenu;
				nbelmnt++;
			}
		}
	}
	//écrire une nouvelle map avec les nouvelles dimensions en y ajoutant lmes anciens éléments

	printf("fin récupération des données\n");
	lseek(file,0,SEEK_END-(oldWidth*oldHeight));
	int realHeight=oldHeight-newHeight;
	//if(realHeight<0){
		//realHeight = abs(realHeight);
		for(int i=0;i<newWidth;i++){
			for(int j=abs(realHeight);j<newHeight;j++){
				for(int indice=0;indice<(oldWidth*oldHeight);indice++){
					if(tabElmnt[indice].x==i && (tabElmnt[indice].y)==(j) && (tabElmnt[indice].value>0)){
						write(file,&tabElmnt[indice].value,sizeof(int));
						tabElmnt[indice].value=-1;
					}
					if(indice==(oldWidth*oldHeight)-1){
						write(file,0,sizeof(int));
					}
				}

			}
		}

	printf("fin réécriture de la map\n");

}


int main(int argc, char** argv){
	printf("ENtree dans main\n");
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
		printf("\nentrée dans --setwidth\n");
		setmap(file,width,height,atoi(argv[3]),height);
		width = atoi(argv[3]);
    lseek(file,0, SEEK_SET);
    write(file,&width, sizeof(int));
    printf("width  : %d\n", width);
  }
  else if (strcmp(argv[2],"--setheight") == 0 ) {
    // on lit la valeur de map_width
    //read(file, &width,sizeof(int));
		printf("\nentrée dans la condition setheight\n");

		setmap(file,width,height,width,atoi(argv[3]));
    height = atoi(argv[3]);
    lseek(file,sizeof(int), SEEK_SET);
    write(file,&height, sizeof(int));
    printf("\nheight  : %d\n", height);
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
