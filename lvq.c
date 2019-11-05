#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define D 2
#define M 5
float HTA = 0.1;

struct point{
	float x[D];
	int group;
};

struct neuron{
	float x[D];
};

float random_(float min,float max){
	float scale = rand() / (float) RAND_MAX;
    return min + scale * ( max - min );
}

const char* getfield(char* line, struct point *p)
{
	(*p).x[0] = atof(strtok(line, " "));
	(*p).x[1] = atof(strtok(NULL, " "));
}

void init_points(struct point *p, int c){
    FILE* stream = fopen("points.dat", "r");
    char line[1024];
    int i;
    for (i=0; i<c; i++) {
        if (fgets(line, 1024, stream)) {
            char* tmp = strdup(line);
            getfield(line, &(p[i]));
            free(tmp);
        }
    }
}

void init_weights(struct neuron *n,struct point *p){
	for (int i=0; i<M; i++){
		int r_number = (int)random_(0,500);
		//printf("%d\n",r_number);
		n[i].x[0]=p[r_number].x[0];
		n[i].x[1]=p[r_number].x[1];
	}
}

void lvq(struct neuron *n, struct point p){
	float array[M];
	for (int i=0; i<M; i++){
		array[i]=sqrt(powf(n[i].x[0]-p.x[0],2)+powf(n[i].x[1]-p.x[1],2));
	}
	float min = array[0];
	int winner=0;
	for (int i=1; i<M; i++){
		if (array[i]<min){
			min = array[i];
			winner = i;
		}
	}
	n[winner].x[0]=n[winner].x[0]+(HTA*(p.x[0]-n[winner].x[0]));
	n[winner].x[1]=n[winner].x[1]+(HTA*(p.x[1]-n[winner].x[1]));
}

int main(){
	struct point p[500];
	struct neuron n[M];
	struct point old_weights[M];
	init_points(p, 500);

	init_weights(n,p);
	for(int e=0; e<10000000; e++){
		for (int j=0;j<M;j++){
			old_weights[j].x[0]=n[j].x[0];
			old_weights[j].x[1]=n[j].x[1];
		}
		for (int i=0; i<500; i++){
			lvq(n,p[i]);
		}
		HTA=HTA-HTA*10/100;
		int counter=0;
		for (int j=0;j<M;j++){
			if (old_weights[j].x[0] == n[j].x[0] && old_weights[j].x[1] == n[j].x[1]){
				counter++;
			}
		}
		//printf ("counter = %d \n",counter);
		if (counter==5){
			printf("terminated after %d epochs\n",e);
			break;
		}
	}
	puts ("centers");
	FILE *f1 = fopen("lvqCenters.dat", "w+");
	for(int i=0; i<M; i++){
		printf ("%f . %f\n",n[i].x[0],n[i].x[1]);
        fprintf(f1, "%f %f\n",n[i].x[0], n[i].x[1]);
	}
    fclose(f1);

}
