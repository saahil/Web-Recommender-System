#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

long double util(int us, int mov, FILE *u, FILE *m) {
	int user, movie, rating;
	int cur_mov;
	int i;
	int cur_pos;
	char *buf;
	char gen_list[20];
	long double num, denom1, denom2, denom, wis, wic, ut;
	rewind(u);
	rewind(m);
	buf = malloc(sizeof(char)*1000);
	num=denom1=denom2=0;

	do {
		memset(buf, 0, 1000);
		cur_pos = ftell(u);
		fgets(buf, 1000, u);
		sscanf(buf, "%d", &user);
	} while(user!=us);

	do {
		fscanf(m, "%d %s\n", &cur_mov, gen_list);
	} while(cur_mov!=mov);

	fseek(u, cur_pos, SEEK_SET);
	fscanf(u, "%d", &user);
	for(i=0; i<18; i++) {
		fscanf(u, "%llf", &wic);
		wis = (long double)(gen_list[i]-'0');
		num += wic*wis;
		denom1 += wic*wic;
		denom2 += wis*wis;
	}

	denom1 = sqrtl(denom1);
	denom2 = sqrtl(denom2);
	if(denom1==0) return 0.0;
	ut = 5*num/(denom1*denom2);

	return ut;
}

int main() {
	long double ut; 
	FILE *u, *m;
	long double mae;
	long double mae_list[5000];
	long double error;
	long double cal_util;
	int mov_n=0;
	int i;
	int cur_user, movie, rating, time;
	FILE *test;
	FILE *result;

	u = fopen("us_profiles.dat", "r");
	m = fopen("main_movies.dat", "r");
	test = fopen("new_testing.dat", "r");
	result = fopen("result.dat", "w");

	mae = 0.0;

	do {
		fscanf(test, "%d::%d::%d::%d\n", &cur_user, &movie, &rating, &time);
		cal_util = util(cur_user, movie, u, m);
		mae_list[mov_n++] = fabsl(rating-cal_util);
		fprintf(result, "%d::%d::%llf\n", cur_user, movie, cal_util);
	} while(!feof(test));

	for(i=0; i<mov_n; i++) {
		mae += mae_list[i];
	}

	printf("\n%d", mov_n);
	mae = mae/((mov_n));
	fprintf(result, "\nMAE:%llf", mae);

	fclose(u); 
	fclose(m);
	fclose(test);
	fclose(result);

	return 0;
}
