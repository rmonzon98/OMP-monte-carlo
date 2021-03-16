#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "my_rand.h"

/* Serial function */
void Get_args(char* argv[], int* thread_count_p,
      long long int* number_of_tosses_p);
void Usage(char* prog_name);

/* Parallel function */
long long int Count_hits(long long int number_of_tosses, int thread_count);

double randomInRange();

/*---------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   double pi_estimate;
   int thread_count;
   long long int number_in_circle;
   long long int number_of_tosses;

   if (argc != 3) Usage(argv[0]);
   Get_args(argv, &thread_count, &number_of_tosses);

   number_in_circle = Count_hits(number_of_tosses, thread_count);
   printf("Número de TIROS/Intentos %lld\n", number_of_tosses);
   printf("Número de aciertos %lld\n", number_in_circle);
   
   pi_estimate = 4*number_in_circle/((double) number_of_tosses);
   printf("Estimated pi: %e\n", pi_estimate);

   return 0;
}
/*---------------------------------------------------------------------
 * Function:      Count_hits
 * Purpose:       Calculate number of hits in the unit circle
 * In arg:        number_of_tosses, thread_count
 * Return val:    number_in_circle
 */

long long int Count_hits(long long int number_of_tosses, int thread_count) {
   long long int hits = 0;
   double x, y, distance;
   int i;
   #pragma omp parallel num_threads(thread_count) shared(seed, xx) private(i) reduction(+: hits)
   {
      #pragma omp for
      for (i = 0; i < number_of_tosses; i++) {
         y = randomInRange(-1.0,1.0);
         x = randomInRange(-1.0,1.0);
         distance = (x*x) + (y*y);
         if(distance<=1.0) {
            hits++;
         }
      }
   }
   return hits;

}  /* Count_hits */

double randomInRange(double a,double b){
    return ((double)rand()*(b-a))/(double)RAND_MAX+a;
}

/*---------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message showing how to run program and quit
 * In arg:    prog_name:  the name of the program from the command line
 */

void Usage(char prog_name[] /* in */) {
   fprintf(stderr, "usage: %s ", prog_name);
   fprintf(stderr, "<number of threads> <total number of tosses>\n");
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argv
 * Out args:    thread_count_p, number_of_tosses_p
 */

void Get_args(
           char*           argv[]              /* in  */,
           int*            thread_count_p      /* out */,
           long long int*  number_of_tosses_p  /* out */) {

   *thread_count_p = strtol(argv[1], NULL, 10);
   *number_of_tosses_p = strtoll(argv[2], NULL, 10);
}  /* Get_args */