#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *in_file;
	FILE *training;
	FILE *testing;
	int cur_user;
	int cur_pos, next_pos;
	int user_n;
	int i;
	int user, movie, rating;
	in_file = fopen("udata.txt", "r");
	training = fopen("training.txt", "w");
	testing = fopen("testing.txt", "w");
	
	while(!feof(in_file)) {
		cur_pos = ftell(in_file);
		fscanf(in_file, "%d %d %d", &cur_user, &movie, &rating);
		user_n = 0;
		user = cur_user;
		printf("\nCurrent user: %d", cur_user);
		while((user==cur_user) && (!feof(in_file))) {
			next_pos = ftell(in_file);
			user_n++;
			fscanf(in_file, "%d %d %d", &user, &movie, &rating);
		}
		if(feof(in_file)) next_pos = -1;
		fseek(in_file, cur_pos, SEEK_SET);
		for(i=0; i<(user_n/2); i++) {
			fscanf(in_file, "%d %d %d", &user, &movie, &rating);
			fprintf(training, "%d %d %d\n", user, movie, rating);
		}
		for(;i<user_n; i++) {
			fscanf(in_file, "%d %d %d", &user, &movie, &rating);
			fprintf(testing, "%d %d %d\n", user, movie, rating);
		}
		if(next_pos==-1) break;
		fseek(in_file, next_pos, SEEK_SET);
	}

	fclose(in_file);
	fclose(training);
	fclose(testing);

	return 0;
}
