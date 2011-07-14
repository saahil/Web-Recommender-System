#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int uid; 
	long double s;
} users;

long double sim(int x, int y, FILE *arg_file) {
	int cur_user;
	int x_age, x_occ;
	long x_zip;
	char x_sex;
	int y_age, y_occ;
	long y_zip;
	char y_sex;
	int num=0;

	printf("\nHere for %d and %d", x, y);
	fseek(arg_file, 0, SEEK_SET);
	do {
		fscanf(arg_file, "%d::%c::%d::%d::%ld", &cur_user, &x_sex, &x_age, &x_occ, &x_zip);
	}while((cur_user!=x) && (!feof(arg_file)));

	fseek(arg_file, 0, SEEK_SET);
	
	do {
		fscanf(arg_file, "%d::%c::%d::%d::%ld", &cur_user, &y_sex, &y_age, &y_occ, &y_zip);
	}while((cur_user!=y) && (!feof(arg_file)));

	if(x_sex==y_sex) num += 1.0;
	if(x_age==y_age) num += 1.0;
	if(x_occ==y_occ) num += 1.0;
	if(x_zip==y_zip) num += 1.0;

	return (num/4.0);
	
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
	users us[500];
	int user;
	int cur_user;
	int i;
	int us_n;
	int max_user;
	FILE* in_file; 
	FILE* out_file;
	FILE *arg_file;
	in_file = fopen("new_users.dat", "r");
	out_file = fopen("neighbours.dat", "w");
	arg_file = fopen("new_users.dat", "r");
	buf=malloc(sizeof(char)*1000);
	while(!feof(in_file)) {
		us_n=0;
		memset(buf, 0, 1000);
		fgets(buf, 1000, in_file);
		cur_pos = ftell(in_file);
		sscanf(buf, "%d", &user);
		fseek(in_file, 0, SEEK_SET);
		fprintf(out_file, "%d->", user);
		while(1) {
			memset(buf, 0, 1000);
			fgets(buf, 1000, in_file);
			if(feof(in_file)) break;
			sscanf(buf, "%d", &cur_user);
			if(cur_user!=user) {
				printf("\nCalculating sim between %d and %d", user, cur_user);
				cur_sim = sim(user, cur_user, arg_file);
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
	fclose(arg_file);
	return 0;
}
