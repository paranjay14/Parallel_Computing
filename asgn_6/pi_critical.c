#include<stdio.h>
#include <stdlib.h>
#include<omp.h>
#include<time.h>

int main(int argc, char* argv[]){

	int num_threads = atoi(argv[2]);
	omp_set_num_threads(num_threads);

	int num_steps=atoi(argv[1]), i=0;
	double start, stop, pi_val, sum=0.0, step_val;
	step_val = 1.0/(double)num_steps;

	start = omp_get_wtime();
	#pragma omp parallel
	{	
		double x, tmp_sum=0.0;
		#pragma omp for schedule(static)
			for (i = 0; i < num_steps; ++i){
				x = ((double)i+0.5) * step_val;
				tmp_sum += (4.0 / (1.0 + x*x));
			}

		#pragma omp critical
			sum += tmp_sum;
	}
	pi_val = step_val*sum;
	stop = omp_get_wtime();
	
	// printf("%G\n", pi_val);
	printf("%G", stop - start);

	return 0;
}
