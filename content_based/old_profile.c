#include <stdio.h>
#include <stdlib.h>

long double gim(int us, int gen, FILE *u, FILE *m) {
	long double g;
	int user, movie, rating, time;
	int cur_movie;
	int gr, tr;
	long double rgr;
	int tf=0;
	int i;
	long double mrgf=0.0;
	char gen_list[20];
	gr = 0; 
	tr = 0;

	rewind(u);
	do {
		fscanf(u, "%d %d %d", &user, &movie, &rating);
	} while(user!=us);

	do {
		tf++;
		tr += rating;
		rewind(m);

		if(rating>=3) {
			do {
				fscanf(m, "%d %s", &cur_movie, gen_list);
			}while((cur_movie!=movie) && (!feof(m)));

			if(gen_list[gen]=='1') {
				gr += rating;
				if(rating==3) mrgf += 1.0;
				else if (rating==4) mrgf += 2.0;
				else mrgf += 3.0;
			}
		}

		fscanf(u, "%d %d %d", &user, &movie, &rating);
	} while((user==us) && (!feof(u)));

	mrgf = mrgf/((long double)3*(long double)tf);
	rgr = (long double)gr/tr;
	
	if((mrgf==0) || (rgr==0)) return 0.0;
	g = 2.0*rgr*mrgf/(rgr+mrgf);

	return g;
}

int main() {
	long double gimi;
	FILE *u, *m;
	int gen_i;
	FILE *ratings, *profile;
	int user, movie, rating, time;
	int cur_user; 
	ratings = fopen("training.txt", "r");
	profile =fopen("us_profiles.dat", "w");
	u = fopen("training.txt", "r");
	m = fopen("new1.dat", "r");

	fscanf(ratings, "%d %d %d\n", &cur_user, &movie, &rating);
	while(!feof(ratings)) {
		user = cur_user;
		printf("\nFor user: %d", user);
		fprintf(profile, "%d", user); 
		for(gen_i=0; gen_i<18; gen_i++) {
			fprintf(profile, " %llf", gim(user, gen_i, u, m));
		}
		fprintf(profile, "\n");
		while((cur_user==user) && (!feof(ratings)))
			fscanf(ratings, "%d %d %d\n", &cur_user, &movie, &rating);
	}

	fclose(u);
	fclose(m);
	fclose(ratings);
	fclose(profile);
	return 0;
}
