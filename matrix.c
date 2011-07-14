/*****************************************************************************************************
 * This code creates the rating matrix for all the users
 * *****************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *mat, *ratings;
	int user, movie, rating, time;
	int mov_list[10000];
	int n, j;
	int cur_user;
	ratings = fopen("new_training.dat", "r");
	mat = fopen("new_training_matrix.dat", "w");
	fscanf(ratings, "%d::%d::%d::%d", &user, &movie, &rating, &time);
	while(!feof(ratings)) {
		fprintf(mat, "\n%d->", user);
		cur_user = user;
		n=0;

		do {
			if(feof(ratings)) break;
			
			j=n-1;
			while((mov_list[j]>movie) && j>=0) {
				mov_list[j+1] = mov_list[j];
				j--;
			}
			mov_list[j+1]=movie;
			n++;
			
			fscanf(ratings, "%d::%d::%d::%d", &user, &movie, &rating, &time);
		} while(user==cur_user);
		
		for(j=0; j<n; j++)
			fprintf(mat, "::%d", mov_list[j]);
	}
	fclose(ratings);
	fclose(mat);
	return 0;
}
