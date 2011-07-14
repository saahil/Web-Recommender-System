#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
	FILE *con, *cf, *demo, *test;
	int user, movie, time; 
	int n=0;
	long double cf_rating, con_rating, demo_rating;
	int act_rating;
	long double mae;
	long double cf_w, con_w, demo_w;
	long double combined;

	con = fopen("con_result.dat", "r");
	cf = fopen("cf_result.dat", "r");
	demo = fopen("demo_result.dat", "r");
	test = fopen("new_testing.dat", "r");
	cf_w = 0.46;
	con_w = 0.05;
	demo_w = 0.49;
	mae = 0.0;

	while(!feof(test)) {
		fscanf(test, "%d::%d::%d::%d\n", &user, &movie, &act_rating, &time);
		fscanf(con, "%d::%d::%llf", &user, &movie, &con_rating);
		fscanf(cf, "%d::%d::%llf", &user, &movie, &cf_rating);
		fscanf(demo, "%d::%d::%llf", &user, &movie, &demo_rating);
		combined = (cf_w*cf_rating)+(con_w*con_rating)+(demo_w*demo_rating);
		printf("%llf %llf %llf %llf %d\n", cf_rating, con_rating, demo_rating, combined, act_rating);
		mae += fabsl((long double)(act_rating)-combined);
		n++;
	}

	mae = mae/n;

	printf("\nMAE:%llf", mae);

	return 0;
}
