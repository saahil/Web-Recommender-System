/*****************************************************************************************************
 * Use the neighbourhood set to predict ratings for the user based on Resnick's formula
 * *****************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	int uid;
	long double sim;
	long double avg;
	int rating;
} users;

long double aggr(int us, int mov, FILE *neigh, FILE *train) {
	int user, movie, rating, time;
	int cur_user;
	long double cur_sim;
	char *buf;
	char ch;
	int us_pos;
	int us_i;
	int c_i;
	int i;
	int found;
	long double avg_user;
	long double k=0.0;
	long double res=0.0;
	users c[30];
	buf = malloc(sizeof(char)*1000);
	rewind(neigh); 
	rewind(train);
	cur_user=0;
	c_i = 0;

	/*****************************************************************************************************
	 * Find the given user's neighbours
	 * *****************************************************************************************************/
	while(cur_user!=us) {
		us_pos=ftell(neigh);
		fgets(buf, 1000, neigh);
		sscanf(buf, "%d->", &cur_user);
	}
	fseek(neigh, us_pos, SEEK_SET);
	fscanf(neigh, "%d->", &user);\
	ch = fgetc(neigh);

	/*****************************************************************************************************
	 * Create the set c. C contains the neighbouring users of us who've rated movie mov
	 * *****************************************************************************************************/
	while(ch!='\n') {
		avg_user = 0;
		us_i = 0;
		fscanf(neigh, ":%d;%llf", &cur_user, &cur_sim);
		rewind(train);
		fscanf(train, "%d::%d::%d::%d", &user, &movie, &rating, &time);
		found=0;
		while((user<=cur_user) && (!feof(train))) {
			if(user==cur_user) {
				avg_user += (long double)(rating);
				us_i++;
				if(movie==mov) {
					found=1;
					c[c_i].uid = cur_user;
					c[c_i].sim = cur_sim;
					c[c_i].rating = rating;
				}
			}
			fscanf(train, "%d::%d::%d::%d", &user, &movie, &rating, &time);
		}
		if(found==1) {
			avg_user = avg_user/((long double)(us_i));
			c[c_i++].avg = avg_user;
		}
		ch = fgetc(neigh);
	}

	/*****************************************************************************************************
	 * Find the average rating given by user us
	 * *****************************************************************************************************/
	rewind(train);
	avg_user = 0;
	us_i = 0;
	fscanf(train, "%d::%d::%d::%d", &user, &movie, &rating, &time);
	while((user<=us) && (!feof(train))) {
		if(user==us) {
			avg_user += (long double)(rating);
			us_i++;
		}
		fscanf(train, "%d::%d::%d::%d", &user, &movie, &rating, &time);
	}
	avg_user = avg_user/((long double)(us_i));

	for(i=0; i<c_i; i++) {
		res += c[i].sim * ((long double)(c[i].rating)-c[i].avg);
		k += fabsl(c[i].sim);
	}

	if(k==0) return avg_user;
	k = (long double)(1.0)/k;
	res = res*k;
	res += avg_user;

	return res;
}

int main() {
	FILE *neigh; 
	FILE *train;
	FILE *test;
	int user, movie, rating, time;
	long double pred;
	long double mae;
	int n;
	int user_pos;
	int cur_user;
	long double mae_list[500];
	int mae_i=0;
	test = fopen("new_testing.dat", "r");
	neigh = fopen("neighbours.dat", "r");
	train = fopen("new_training.dat", "r");
	
	while(!feof(test)) {
		fscanf(test, "%d::%d::%d::%d\n", &user, &movie, &rating, &time);
		mae  = 0.0;
		n = 0;
		cur_user = user;

		while(user==cur_user) {
			pred = aggr(user, movie, neigh, train);
			mae += fabsl(pred-rating);
			printf("%d::%d::%llf\n", user, movie, pred);
			n++;
			user_pos=ftell(test);
			if(feof(test)) break;
			fscanf(test, "%d::%d::%d::%d\n", &user, &movie, &rating, &time);
		}
		mae = mae/((long double)(n));

		mae_list[mae_i++] = mae;
		if(feof(test)) break;
		fseek(test, user_pos, SEEK_SET);
	}
	
	mae = 0.0;
	for(n=0; n<mae_i; n++) 
		mae += mae_list[n];

	mae = mae/mae_i;
	printf("\n\nMAE:%llf\n", mae);

	return 0;
}
