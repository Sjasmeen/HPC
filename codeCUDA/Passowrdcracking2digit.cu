#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
/****************************************************************************
  This program gives an example of a poor way to implement a password cracker
  in CUDA C. It is poor because it acheives this with just one thread, which
  is obviously not good given the scale of parallelism available to CUDA
  programs.
 
  The intentions of this program are:
    1) Demonstrate the use of __device__ and __global__ functions
    2) Enable a simulation of password cracking in the absence of library
       with equivalent functionality to libcrypt. The password to be found
       is hardcoded into a function called is_a_match.   

  Compile and run with:
  nvcc -o Passwordcracking2digit Passwordcracking2digit.cu


     To Run:
     ./Passwordcracking2digit > resultscuda_2alp2dig.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

/****************************************************************************
  This function returns 1 if the attempt at cracking the password is
  identical to the plain text password string stored in the program.
  Otherwise,it returns 0.
*****************************************************************************/
__device__ int is_a_match(char *attempt) {
  char p1[] = "CV75";
  char p2[] = "FR51";
  char p3[] = "TB51";
  char p4[] = "IS95";

  char *w = attempt;
  char *x = attempt;
  char *y = attempt;
  char *z = attempt;
  char *password1 = p1;
  char *password2 = p2;
  char *password3 = p3;
  char *password4 = p4;

  while(*w == *password1) {
   if(*w == '\0')
    {
    printf("Found password: %s\n",p1);
      break;
    }

    w++;
    password1++;
  }
    
  while(*x == *password2) {
   if(*x == '\0')
    {
    printf("Found password: %s\n",p2);
      break;
}

    x++;
    password2++;
  }

  while(*y == *password3) {
   if(*y == '\0')
    {
    printf("Found password: %s\n",p3);
      break;
    }

    y++;
    password3++;
  }

  while(*z == *password4) {
   if(*z == '\0')
    {
    printf("Found password: %s\n",p4);
      return 1;
    }

    z++;
    password4++;
  }
  return 0;

}
/****************************************************************************
  The kernel function assume that there will be only one thread and uses
  nested loops to generate all possible passwords and test whether they match
  the hidden password.
*****************************************************************************/

__global__ void  kernel() {
char a,b;
 
  char password[5];
  password[4] = '\0';

int i = blockIdx.x+65;
int j = threadIdx.x+65;
char firstValue = i;
char secondValue = j;
    
password[0] = firstValue;
password[1] = secondValue;
    for(a='0'; a<='9'; a++){
      for(b='0'; b<='9'; b++){
            password[2] = a;
            password[3] = b;
          if(is_a_match(password)) {
        //printf("Success");
          }
             else {
         //printf("tried: %s\n", password);          
            }
          }
        } 
      
}
int time_difference(struct timespec *start,
                    struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec;
  long long int dn =  finish->tv_nsec - start->tv_nsec;

  if(dn < 0 ) {
    ds--;
    dn += 1000000000;
  }
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


int main() {

  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

kernel <<<26,26>>>();
  cudaThreadSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));
  return 0;
}




