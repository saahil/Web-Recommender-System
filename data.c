/*****************************************************************************************************
 * This code just divides the main bigger dataset into smaller chunks for our use
 * *****************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *ratings, *new_ratings; 
	int user, movie, rating, time;
	ratings = fopen("ratings.dat", "r");
	new_ratings = fopen("new_ratings.dat", "w");
	fscanf(ratings, "%d::%d::%d::%d", &user, &movie, &rating, &time);
	while(user<=100) {
		if(movie<=1500) fprintf(new_ratings, "%d::%d::%d::%d\n", user, movie, rating, time);
		fscanf(ratings, "%d::%d::%d::%d", &user, &movie, &rating, &time);
	}
	fclose(ratings);
	fclose(new_ratings);
	return 0;
}

