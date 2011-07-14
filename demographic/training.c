/*****************************************************************************************************
 * This code creates the training and testing data set
 * *****************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *in_file, *train_file, *test_file;
	int user_n, i;
	int user_start, cur_pos, next_user;
	int cur_user;
	int user, movie, rating, time;
	in_file = fopen("new_ratings.dat", "r");
	train_file = fopen("new_training.dat", "w");
	test_file = fopen("new_testing.dat", "w");
	next_user=0;
	do {
		user_n=0;
		fscanf(in_file, "%d::%d::%d::%d", &user, &movie, &rating, &time);
		cur_user = user;
		while((cur_user==user) && (!feof(in_file))) {
			user_n++;
			cur_pos = ftell(in_file);
			fscanf(in_file, "%d::%d::%d::%d", &user, &movie, &rating, &time);
		}
		next_user = cur_pos;
		if(feof(in_file)) next_user=-1;
		fseek(in_file, user_start, SEEK_SET);
		for(i=0; i<(user_n/2); i++) {
			fscanf(in_file, "%d::%d::%d::%d", &user, &movie, &rating, &time);
			fprintf(train_file, "%d::%d::%d::%d\n", user, movie, rating, time);
		}
		for(;i<user_n; i++) {
			fscanf(in_file, "%d::%d::%d::%d", &user, &movie, &rating, &time);
			fprintf(test_file, "%d::%d::%d::%d\n", user, movie, rating, time);
		}
		if(next_user==-1) break;
		fseek(in_file, next_user, SEEK_SET);
		user_start = next_user;
	}while(1);

	fclose(in_file);
	fclose(train_file);
	fclose(test_file);
	return 0;
}

