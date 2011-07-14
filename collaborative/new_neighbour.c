#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	int uid; 
	long double s;
} users;

long double sim(int x, int y, FILE *x_rating, FILE *y_rating, FILE *x_mat, FILE *y_mat) {
	char *x_buf, *y_buf;
	char dum[100];
	char c, d;
	int user_x, movie_x, rating_x, time_x;
	int user_y, movie_y, rating_y, time_y;
	int x_pos, y_pos;
	int n_x, n_y; 
	int sxy[100];
	int x_tot, y_tot;
	int i;
	long double avg_x, avg_y;
	long double num, denom1, denom2, denom;
	long double simi;
	int cur_x, cur_y;
	int s_i=0;
	rewind(x_rating);
	rewind(y_rating);
	rewind(x_mat);
	rewind(y_mat);
	x_buf = malloc(sizeof(char)*5000);
	y_buf = malloc(sizeof(char)*5000);
	x_pos = y_pos = 0;
	avg_x = avg_y = 0.0;
	n_x = n_y = 0;
	i=0;
	num=denom1=denom2=denom=0.0;

	/*****************************************************************************************************
	 * Look for x's entry in the matrix
	 * *****************************************************************************************************/
	do {
		memset(x_buf, 0, 5000);
		x_pos = ftell(x_mat);
		fgets(x_buf, 5000, x_mat);
		sscanf(x_buf, "%d->", &user_x);
	} while(user_x != x);
	sprintf(dum, "%d", x);
	x_pos += strlen(dum)+2; /* Bring back the x file pointer to the beginning of first movie in x*/
	fseek(x_mat, x_pos, SEEK_SET);

	/*****************************************************************************************************
	 * Look for y's entry in the matrix
	 * *****************************************************************************************************/
	do {
		memset(y_buf, 0, 5000);
		y_pos = ftell(y_mat);
		fgets(y_buf, 5000, y_mat);
		sscanf(y_buf, "%d->", &user_y);
	} while(user_y != y);
	sprintf(dum, "%d", y);
	y_pos += strlen(dum)+2; /* Bring back the y file pointer to the beginning of first movie in y*/ 
	fseek(y_mat, y_pos, SEEK_SET);
	
	/*****************************************************************************************************
	 * Start filling the Sxy set
	 * *****************************************************************************************************/
	c = fgetc(x_mat);
	d = fgetc(y_mat);
	fscanf(x_mat, ":%d", &movie_x);
	fscanf(y_mat, ":%d", &movie_y);
	while((c != '\n') && (d != '\n')) {
		while((movie_y<movie_x)) { 
			d=fgetc(y_mat);
			if(d=='\n') break;
			fscanf(y_mat, ":%d", &movie_y);
		}
		while((movie_x<movie_y)) {
			c=fgetc(x_mat);
			if(c=='\n') break;
			fscanf(x_mat, ":%d", &movie_x);
		}
		if(movie_x==movie_y) {
			sxy[s_i++] = movie_x;
			c = fgetc(x_mat);
			d = fgetc(y_mat);
			if((c=='\n') || (d=='\n')) break;
			fscanf(x_mat, ":%d", &movie_x);
			fscanf(y_mat, ":%d", &movie_y);
		}
	}
	
	/*****************************************************************************************************
	 * Look for x's entry in the ratings recordset
	 * *****************************************************************************************************/
	do {
		x_pos = ftell(x_rating);
		fscanf(x_rating, "%d::%d::%d::%d", &user_x, &movie_x, &rating_x, &time_x);
	} while(user_x!=x);
	
	/*****************************************************************************************************
	 * Look for y's entry in the ratings recordset
	 * *****************************************************************************************************/
	do {
		y_pos = ftell(y_rating);
		fscanf(y_rating, "%d::%d::%d::%d", &user_y, &movie_y, &rating_y, &time_y);
	} while(user_y!=y);

	/*****************************************************************************************************
	 * Averaging the ratings given by x 
	 * *****************************************************************************************************/
	fseek(x_rating, x_pos, SEEK_SET);
	fscanf(x_rating, "%d::%d::%d::%d\n", &user_x, &movie_x, &rating_x, &time_x);
	while(user_x==x) {
		avg_x += (long double)(rating_x);
		n_x++;
		if(feof(x_rating)) break;
		fscanf(x_rating, "%d::%d::%d::%d\n", &user_x, &movie_x, &rating_x, &time_x);
	}
	avg_x = avg_x/n_x;

	/*****************************************************************************************************
	 * Averaging the ratings given by y
	 * *****************************************************************************************************/
	fseek(y_rating, y_pos, SEEK_SET);
	fscanf(y_rating, "%d::%d::%d::%d\n", &user_y, &movie_y, &rating_y, &time_y);
	while(user_y==y) {
		avg_y += (long double)(rating_y);
		n_y++;
		if(feof(y_rating)) break;
		fscanf(y_rating, "%d::%d::%d::%d\n", &user_y, &movie_y, &rating_y, &time_y);
	};
	avg_y = avg_y/n_y;
	
	/*****************************************************************************************************
	 * Calculating similarity now
	 * *****************************************************************************************************/
	for(i=0; i<s_i; i++) {
		fseek(x_rating, x_pos, SEEK_SET);
		fseek(y_rating, y_pos, SEEK_SET);
		do {
			fscanf(x_rating, "%d::%d::%d::%d", &user_x, &movie_x, &rating_x, &time_x);
		}while(movie_x!=sxy[i]);
		
		do {
			fscanf(y_rating, "%d::%d::%d::%d", &user_y, &movie_y, &rating_y, &time_y);
		}while(movie_y!=sxy[i]);

		num += ((long double)(rating_x)-avg_x)*((long double)(rating_y)-avg_y);
		denom1 += ((long double)(rating_x)-avg_x)*((long double)(rating_x)-avg_x);
		denom2 += ((long double)(rating_y)-avg_y)*((long double)(rating_y)-avg_y);
	}

	denom = sqrtf(denom1*denom2);

	if(denom==0) return -2;
	simi = num/denom;

	return simi;
}

void sort(users us[], int us_i) {
	int i, j;
	users cur_user;
	for(i=1; i<us_i; i++) {
		cur_user.uid = us[i].uid;
		cur_user.s = us[i].s;
		j = i-1;
		while((cur_user.s>=us[j].s) && j>=0){
			us[j+1].uid = us[j].uid;
			us[j+1].s = us[j].s;
			j--;
		}
		us[j+1].uid = cur_user.uid;
		us[j+1].s = cur_user.s;
	}
}

int main() {
	long double cur_sim; 
	int cur_pos=0;
	char* buf;
	users us[200];
	int user, movie, rating, time;
	int cur_user;
	int i;
	int us_n;
	int max_user;
	FILE* in_file; 
	FILE* out_file;
	FILE *x_rating, *y_rating;
	FILE *x_mat, *y_mat;
	in_file = fopen("new_training_matrix.dat", "r");
	out_file = fopen("new_neighbours.dat", "w");
	x_rating = fopen("new_training.dat", "r");
	y_rating = fopen("new_training.dat", "r");
	x_mat = fopen("new_training_matrix.dat", "r");
	y_mat = fopen("new_training_matrix.dat", "r");
	buf=malloc(sizeof(char)*1000);
	while(!feof(in_file)) {
		us_n = 0;
		memset(buf, 0, 1000);
		fgets(buf, 1000, in_file);
		cur_pos = ftell(in_file);
		sscanf(buf, "%d->", &user);
		fseek(in_file, 0, SEEK_SET);
		fprintf(out_file, "%d->", user);
		while(1) {
			memset(buf, 0, 1000);
			fgets(buf, 1000, in_file);
			if(feof(in_file)) break;
			sscanf(buf, "%d->", &cur_user);
			if(cur_user!=user) {
				printf("\nCalculating sim between %d and %d", user, cur_user);
				cur_sim = sim(user, cur_user, x_rating, y_rating, x_mat, y_mat);
				if(cur_sim!=-2) {
					us[us_n].uid = cur_user;
					us[us_n++].s = cur_sim;
				}
			}
			max_user = cur_user;
		}
		sort(us, us_n);
		for(i=0; ((i<20) && (i<us_n)); i++) {
			fprintf(out_file, "::%d;%llf", us[i].uid, us[i].s);
		}
		fprintf(out_file, "\n");
		if(user==max_user) break;
		fseek(in_file, cur_pos, SEEK_SET);
	}

	fclose(in_file);
	fclose(out_file);
	fclose(x_rating);
	fclose(y_rating);
	fclose(x_mat);
	fclose(y_mat);
	return 0;
}
