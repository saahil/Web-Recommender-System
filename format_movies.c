#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************************************
 * Format the movies' description in the form of genre vectors
 * *****************************************************************************************************/

int main() {
	FILE *main;
	FILE *target;
	int genre[18];
	char movie[100];
	int id;
	int gen_len;
	int i;
	int j;
	char c, d;
	char list[20][20];
	main = fopen("movies.dat", "r");
	target = fopen("main_movies.dat", "w");
	
	while(!feof(main)) {
		fscanf(main, "%d::", &id);
		if(id==3952) break;
		fprintf(target, "%d ", id);
		i = 0;
		c = fgetc(main);
		d = fgetc(main);
		while(!((c==':') && (d==':'))) {
			ungetc((int)d, main);
			movie[i++] = c;
			c = fgetc(main);
			d = fgetc(main);
		}
		movie[i] = '\0';
		printf("\nMovie: %s", movie);
		i=j=0;
		while((c=fgetc(main))!='\n') {
			printf("%c", c);
			if(c=='|') {
				list[i][j] = '\0';
				i++;
				j=0;
				printf("\n%s\n", list[i-1]);
			}
			else {
				list[i][j++] = c;
			}
		} 
		list[i++][j] = '\0';
		printf("\n%s", list[i-1]);
		for(j=0; j<18; j++)
			genre[j] = 0;
		for(j=0; j<i; j++) {
			//printf("%s\n", list[i]);
			if(!strcmp(list[j], "Action")) genre[0] = 1;
			else if(!strcmp(list[j], "Adventure")) genre[1] = 1;
			else if(!strcmp(list[j], "Animation")) genre[2] = 1;
			else if(!strcmp(list[j], "Children's")) genre[3] = 1;
			else if(!strcmp(list[j], "Comedy")) genre[4] = 1;
			else if(!strcmp(list[j], "Crime")) genre[5] = 1;
			else if(!strcmp(list[j], "Documentary")) genre[6] = 1;
			else if(!strcmp(list[j], "Drama")) genre[7] = 1;
			else if(!strcmp(list[j], "Fantasy")) genre[8] = 1;
			else if(!strcmp(list[j], "Film-Noir")) genre[9] = 1;
			else if(!strcmp(list[j], "Horror")) genre[10] = 1;
			else if(!strcmp(list[j], "Musical")) genre[11] = 1;
			else if(!strcmp(list[j], "Mystery")) genre[12] = 1;
			else if(!strcmp(list[j], "Romance")) genre[13] = 1;
			else if(!strcmp(list[j], "Sci-Fi")) genre[14] = 1;
			else if(!strcmp(list[j], "Thriller")) genre[15] = 1;
			else if(!strcmp(list[j], "War")) genre[16] = 1;
			else if(!strcmp(list[j], "Western")) genre[17] = 1;
		}

		for(i=0; i<18; i++)
			fprintf(target, "%d", genre[i]);
		fprintf(target, "\n");
		if(feof(main)) break;
	}

	fclose(main);
	fclose(target);

	return 0;
}
