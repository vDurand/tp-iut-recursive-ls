#include <stdio.h>

#include <sys/dir.h>
#include <sys/stat.h>

int status (char chem[]);
void dir (char chem[]);

int status (char chem[]){
	struct stat st;
	int rep;
	int val;

	/* recuperation des infos du i_noeud par stat */
	rep = stat(chem,&st);
	if(rep == 0) {
		//printf("%s : ino = %d\ttaille = %d\t",chem,st.st_ino,st.st_size);
		if(S_ISDIR(st.st_mode)){
			printf(" \tDIRECTORY ");
			val = 1;
		}
		if(S_ISREG(st.st_mode)){
			printf(" \tFICHIER ");
			val = 0;
		} 
		if(S_ISLNK(st.st_mode)){
			printf(" \tLIEN ");
			val = 0;	
		} 
		//printf("owner = %d\tgrpe = %d\n",st.st_uid,st.st_gid);
	}
	else {
	printf("erreur sur stat de %s !!!\n");
	exit(0);
	}
	return val;
}

void lsrecur (char chem[]){
	DIR* dir;
	struct direct* dirdet;
	char* chem2;
	chem2 = (char*)malloc(strlen(chem) +25);
	strcpy(chem2,chem);

	/* ouverture du repertoire et controle d'existence */
	dir = opendir(chem);
	if(dir == NULL) {
	printf("erreur opendir de %s !!!\n",chem);
	exit(0);
	}
	/* lecture des donnees du repertoire */
	while((dirdet = readdir(dir)) != NULL) {
		chem2 = (char*)malloc(strlen(chem) +25);
	strcpy(chem2,chem);
		printf("%s : N ino = %d\tNom = %s",chem,dirdet->d_ino,dirdet->d_name);
		
		if((*(dirdet->d_name)!='.')&&(*(dirdet->d_name)!='..')){
			strcat(chem2,"/");
			strcat(chem2,dirdet->d_name);
			if(status(chem2)==1){
				printf("\n");
				lsrecur(chem2);
			}
			else{
				printf("\n");
				strcpy(chem2,chem);
			}
		}
		else{
			printf(" \tDIRECTORY \n");
		}
	}
	/* liberation memoire */
	free(chem);
	/* fermeture du repertoire ouvert */
	closedir(dir);
}

main(int argc, char *argv[]) {

	char* chem;

	/* controle des parametres d'appel */
	if(argc != 2) {
		printf("erreur de syntaxe d'appel !!!\n");
		exit(0);
	}

	/* recuperation du nom du repertoire */
	chem = (char*)malloc(strlen(argv[1]) +1);
	strcpy(chem,argv[1]);

	lsrecur(chem);
}