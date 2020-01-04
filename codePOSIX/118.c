#include <stdio.h>
#include <math.h>
#include <time.h>

/******************************************************************************
 * This program takes an initial estimate of m and c and finds the associated 
 * rms error. It is then as a base to generate and evaluate 8 new estimates, 
 * which are steps in different directions in m-c space. The best estimate is 
 * then used as the base for another iteration of "generate and evaluate". This 
 * continues until none of the new estimates are better than the base. This is
 * a gradient search for a minimum in mc-space.
 * 
 * To compile:
 *   cc -o 118 118.c -lm
 * 
 * To run:
 *   ./118 > linear.csv | grep Time
 * 
 * Dr Kevan Buckley, University of Wolverhampton, 2018
 *****************************************************************************/

typedef struct point_t {
  double x;
  double y;
} point_t;

int n_data = 1000;
point_t data[];

double residual_error(double x, double y, double m, double c) {
  double e = (m * x) + c - y;
  return e * e;
}

double rms_error(double m, double c) {
  int i;
  double mean;
  double error_sum = 0;
  
  for(i=0; i<n_data; i++) {
    error_sum += residual_error(data[i].x, data[i].y, m, c);  
  }
  
  mean = error_sum / n_data;
  
  return sqrt(mean);
}

int time_difference(struct timespec *start, struct timespec *finish, 
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

 struct timespec start, finish;   
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

  int i;
  double bm = 1.3;
  double bc = 10;
  double be;
  double dm[8];
  double dc[8];
  double e[8];
  double step = 0.01;
  double best_error = 999999999;
  int best_error_i;
  int minimum_found = 0;
  
  double om[] = {0,1,1, 1, 0,-1,-1,-1};
  double oc[] = {1,1,0,-1,-1,-1, 0, 1};

  be = rms_error(bm, bc);

  while(!minimum_found) {
    for(i=0;i<8;i++) {
      dm[i] = bm + (om[i] * step);
      dc[i] = bc + (oc[i] * step);    
    }
      
    for(i=0;i<8;i++) {
      e[i] = rms_error(dm[i], dc[i]);
      if(e[i] < best_error) {
        best_error = e[i];
        best_error_i = i;
      }
    }

    printf("best m,c is %lf,%lf with error %lf in direction %d\n", 
      dm[best_error_i], dc[best_error_i], best_error, best_error_i);
    if(best_error < be) {
      be = best_error;
      bm = dm[best_error_i];
      bc = dc[best_error_i];
    } else {
      minimum_found = 1;
    }
  }
  printf("minimum m,c is %lf,%lf with error %lf\n", bm, bc, be);

clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9));
  return 0;
}

point_t data[] = {
  {68.96,127.53},{65.01,85.76},{87.32,132.24},{85.08,120.53},
  {68.86,126.80},{65.62,111.97},{76.46,129.69},{73.83,107.29},
  {45.02,87.66},{68.95,100.22},{72.95,127.57},{69.97,114.45},
  {69.74,105.81},{71.56,105.36},{66.04,99.73},{72.47,109.18},
  {95.34,132.58},{24.33,60.81},{65.78,100.00},{ 7.31,35.35},
  {88.70,127.68},{ 9.06,50.60},{63.57,108.82},{71.06,105.91},
  {35.08,75.71},{ 5.40,49.77},{16.95,55.92},{85.51,118.30},
  { 2.40,59.04},{95.63,144.59},{87.35,115.66},{92.97,137.81},
  {95.75,139.55},{86.47,126.39},{83.66,133.68},{28.64,74.53},
  {47.87,89.95},{ 8.28,32.43},{85.34,132.68},{38.21,44.45},
  {95.68,125.22},{76.39,118.03},{90.89,122.64},{64.59,88.16},
  {17.45,49.26},{88.35,122.50},{97.29,159.00},{13.76,37.25},
  {44.96,93.57},{86.97,126.46},{26.71,54.98},{66.52,104.86},
  {27.72,68.65},{45.97,92.40},{61.71,105.62},{23.82,60.72},
  {57.51,104.36},{44.17,84.64},{59.08,95.61},{24.68,57.64},
  {62.27,113.18},{65.89,99.82},{33.56,67.81},{93.29,144.45},
  {74.46,113.15},{26.44,75.49},{88.38,142.05},{72.05,102.19},
  {89.67,143.23},{47.87,103.92},{77.77,125.90},{85.02,143.76},
  {19.86,50.69},{73.26,122.63},{58.81,100.48},{55.19,78.87},
  {86.07,147.56},{48.11,86.36},{38.07,78.48},{57.03,102.88},
  {34.27,79.25},{89.14,147.54},{69.68,129.18},{14.69,51.37},
  { 5.53,37.91},{52.35,77.93},{78.30,135.53},{73.35,105.80},
  { 5.31,38.47},{ 9.45,36.88},{17.71,47.50},{63.41,94.28},
  {97.90,141.88},{54.39,100.17},{93.88,131.56},{97.73,147.23},
  {75.54,117.14},{76.13,135.50},{51.16,86.67},{51.78,92.42},
  {50.60,92.68},{53.41,99.78},{96.79,133.74},{16.54,38.54},
  { 5.74,32.37},{16.56,46.26},{70.19,100.04},{30.91,72.43},
  {84.11,156.25},{23.03,64.47},{56.39,86.68},{19.27,68.88},
  {13.92,60.07},{96.69,147.94},{46.97,95.63},{41.90,77.09},
  {67.70,110.96},{93.23,149.54},{20.48,63.12},{71.10,119.84},
  {32.40,68.30},{85.18,126.57},{86.91,126.74},{24.29,61.76},
  { 7.57,29.04},{53.31,106.04},{47.42,104.29},{18.03,59.08},
  {29.16,80.23},{99.86,149.00},{64.70,105.44},{53.10,109.96},
  {89.31,142.34},{90.07,138.83},{93.94,141.90},{41.06,75.52},
  {64.16,120.61},{59.67,94.68},{73.53,96.35},{32.83,76.15},
  { 2.15,17.50},{40.67,70.85},{ 3.69,52.57},{72.86,110.61},
  { 4.69,52.94},{79.66,129.22},{61.24,104.26},{60.06,105.03},
  {63.95,90.36},{ 8.54,24.83},{37.27,88.49},{35.28,69.49},
  { 1.63,56.98},{61.10,111.69},{ 6.61,56.58},{ 2.36,29.57},
  {72.06,121.00},{42.28,78.50},{78.45,127.41},{34.64,89.91},
  {73.45,139.16},{ 0.51,44.90},{91.48,165.90},{27.37,61.15},
  {33.80,72.12},{52.21,87.56},{99.03,160.18},{37.29,72.46},
  {48.24,98.94},{11.87,50.81},{81.70,110.47},{60.23,98.78},
  {16.63,66.26},{24.27,71.76},{99.53,131.86},{75.14,115.86},
  {18.63,42.32},{65.68,107.98},{11.98,63.83},{91.97,125.57},
  {64.16,100.09},{11.31,39.43},{14.60,50.13},{65.90,105.92},
  {67.79,102.00},{48.40,98.05},{ 5.73,43.72},{40.18,64.53},
  {97.11,154.61},{84.39,124.80},{75.42,131.44},{31.79,62.32},
  {61.98,77.29},{88.47,147.40},{58.14,105.39},{68.76,124.48},
  {81.44,128.43},{75.76,138.16},{81.65,135.84},{73.71,114.01},
  {27.58,61.17},{75.96,142.08},{76.34,115.53},{ 8.29,46.24},
  {79.85,136.90},{43.94,109.71},{86.56,123.14},{89.91,138.47},
  {33.08,76.88},{32.44,83.79},{14.62,39.60},{51.16,77.77},
  {89.31,130.62},{88.49,129.58},{18.82,53.62},{19.32,61.80},
  {33.04,71.10},{37.12,63.13},{65.92,109.67},{84.93,131.02},
  {61.27,109.78},{62.67,112.65},{75.64,122.50},{55.05,87.99},
  {46.89,85.81},{48.24,98.30},{53.21,95.11},{41.53,65.36},
  {47.44,88.03},{58.20,120.96},{25.08,60.30},{24.15,55.72},
  {19.14,54.67},{79.71,146.44},{99.99,164.00},{71.63,105.81},
  {53.70,97.55},{77.19,123.67},{23.40,82.08},{67.81,111.97},
  { 4.15,26.88},{11.43,34.86},{34.23,88.30},{38.68,78.98},
  {59.86,116.72},{99.99,144.33},{64.87,107.33},{88.26,136.75},
  {48.78,90.87},{47.84,93.68},{89.39,140.84},{ 8.73,35.47},
  {34.62,78.93},{60.41,111.93},{10.88,45.98},{37.95,58.17},
  {18.59,70.39},{99.22,150.22},{64.17,111.38},{22.04,47.32},
  {78.28,130.70},{68.19,103.80},{31.25,72.37},{16.62,47.74},
  {55.87,101.58},{17.34,39.23},{35.69,75.42},{58.62,97.64},
  { 6.52,29.17},{40.58,91.40},{65.55,99.45},{41.15,78.13},
  {62.27,111.89},{11.71,42.01},{75.78,121.16},{19.56,67.95},
  {92.11,147.71},{13.30,50.97},{ 8.66,42.25},{72.26,121.14},
  {62.56,117.12},{36.94,80.80},{28.93,67.37},{76.71,122.06},
  {72.67,128.50},{87.02,131.77},{17.54,48.18},{22.53,79.49},
  {53.04,84.45},{17.77,62.25},{40.29,87.77},{63.05,101.03},
  {73.15,127.17},{26.54,49.42},{66.87,94.86},{73.19,122.96},
  {59.87,93.83},{48.56,93.25},{95.18,140.41},{87.93,144.89},
  {26.72,60.18},{73.98,118.58},{47.11,87.54},{91.70,129.40},
  {21.05,65.09},{39.77,87.22},{15.52,52.98},{85.86,136.30},
  {14.31,63.49},{34.07,73.69},{58.68,81.45},{ 3.90,24.63},
  {12.31,71.15},{23.14,48.98},{49.94,77.10},{23.31,50.64},
  {22.09,53.63},{ 7.58,55.10},{43.46,69.88},{ 9.49,40.86},
  {33.39,71.79},{34.47,69.23},{ 4.20,50.76},{93.26,164.56},
  {36.40,78.93},{92.22,126.15},{82.28,132.66},{63.56,112.14},
  { 7.50,46.47},{72.48,102.94},{12.47,42.55},{45.53,103.53},
  { 5.85,39.90},{62.85,104.22},{ 1.94,29.59},{ 9.02,36.32},
  {78.15,117.25},{93.71,131.84},{35.21,72.45},{ 6.07,45.70},
  {20.68,43.62},{57.40,86.08},{ 5.28,44.22},{85.62,141.01},
  { 9.53,45.18},{18.03,61.91},{50.22,95.06},{ 2.63,50.96},
  {48.49,96.73},{11.12,54.22},{37.12,77.51},{ 5.33,57.55},
  {16.76,47.02},{ 8.03,34.18},{70.23,128.52},{16.56,52.69},
  {63.37,108.58},{83.57,142.86},{91.49,132.78},{ 9.37,51.24},
  {18.78,52.63},{79.98,126.36},{73.47,128.03},{95.86,147.32},
  { 4.73,60.22},{84.14,149.15},{88.99,138.22},{72.60,130.12},
  {88.98,128.37},{26.06,67.67},{35.28,76.42},{58.90,96.32},
  {45.55,75.07},{ 3.29,43.62},{ 4.47,38.75},{50.13,99.31},
  {33.01,81.84},{40.11,84.34},{50.97,100.60},{97.86,158.95},
  {96.50,121.60},{ 9.18,46.59},{19.24,65.28},{76.18,111.45},
  {76.50,119.12},{12.55,63.80},{97.12,133.37},{ 5.93,36.40},
  {33.25,69.64},{84.92,118.79},{66.74,89.72},{92.66,143.98},
  {61.87,110.39},{90.93,148.82},{10.67,33.91},{41.69,69.79},
  { 7.38,45.76},{14.67,47.62},{56.02,79.59},{ 4.78,45.69},
  {28.83,57.34},{22.07,58.37},{53.41,76.67},{43.34,82.03},
  {88.11,125.88},{67.00,104.43},{78.81,112.15},{76.83,136.31},
  { 3.09,34.19},{31.33,75.78},{71.77,107.29},{66.08,85.43},
  { 0.45,21.58},{88.37,141.69},{10.62,29.70},{85.76,126.94},
  {34.79,75.21},{78.52,129.32},{86.33,114.24},{16.52,58.29},
  {39.36,62.89},{15.57,53.76},{59.81,92.10},{58.43,93.88},
  {14.50,61.10},{55.43,104.94},{66.38,96.65},{28.48,79.21},
  {50.99,120.90},{38.07,77.51},{31.14,70.22},{11.99,57.21},
  {41.77,82.32},{79.03,94.12},{19.43,59.39},{79.12,123.58},
  {85.45,118.30},{67.74,109.74},{62.84,102.58},{ 8.40,45.54},
  {87.57,145.11},{26.07,58.81},{63.87,109.66},{47.17,93.24},
  {67.35,108.91},{94.26,128.83},{10.75,62.95},{ 2.51,42.89},
  {45.79,86.02},{32.68,78.07},{ 4.71,33.98},{57.11,73.85},
  {19.18,62.78},{42.38,83.67},{44.88,92.97},{89.97,143.27},
  { 6.76,39.38},{16.41,52.96},{47.36,85.61},{91.90,130.97},
  {56.10,117.91},{86.81,133.28},{92.18,123.40},{16.36,58.04},
  {64.35,101.99},{75.90,124.29},{43.32,83.52},{89.66,135.32},
  {78.56,110.50},{39.59,86.55},{ 7.93,44.82},{46.39,84.26},
  {88.63,140.71},{19.31,57.72},{ 9.16,59.68},{23.16,40.90},
  {93.83,140.28},{53.68,98.26},{ 2.33,46.84},{34.58,72.81},
  {71.27,107.16},{70.72,106.57},{ 2.35,47.62},{20.48,52.38},
  {83.20,126.52},{46.03,76.24},{37.58,74.51},{69.44,131.27},
  {36.64,62.15},{94.83,148.08},{58.65,100.06},{73.16,139.40},
  {84.49,118.86},{ 9.43,52.92},{49.27,92.82},{69.92,121.16},
  {55.95,97.27},{ 7.24,37.61},{40.55,78.33},{41.55,77.72},
  {59.77,122.83},{76.75,102.31},{27.69,80.01},{48.67,90.20},
  {36.85,80.87},{84.52,124.20},{41.71,92.17},{29.19,66.22},
  {74.49,108.35},{29.78,60.30},{16.23,41.24},{ 6.34,19.78},
  {89.98,131.97},{40.26,91.10},{72.53,113.90},{95.16,137.85},
  {60.83,100.78},{58.74,91.69},{24.47,58.96},{83.63,115.99},
  {26.27,61.48},{33.63,74.52},{80.42,113.21},{39.78,82.67},
  {81.95,122.15},{25.35,65.00},{23.53,68.26},{ 7.01,39.79},
  {21.32,56.11},{10.82,38.47},{ 4.62,41.09},{32.92,62.30},
  {88.21,145.20},{66.31,103.93},{46.19,71.34},{80.10,119.02},
  {29.82,63.25},{93.02,148.19},{45.49,70.06},{81.25,113.65},
  {23.57,74.86},{78.64,122.69},{20.22,43.72},{74.17,105.40},
  {67.71,94.15},{17.74,50.42},{95.90,159.78},{54.73,110.59},
  {92.53,146.46},{91.50,138.82},{18.92,63.64},{19.22,35.06},
  {30.02,69.95},{84.45,136.00},{68.04,109.76},{57.38,100.38},
  {71.74,123.01},{19.09,48.68},{86.06,135.82},{70.58,115.69},
  {97.76,139.68},{42.58,91.52},{57.93,92.63},{55.97,87.95},
  {25.56,76.97},{80.24,120.12},{ 2.99,41.31},{56.64,84.16},
  {24.89,66.51},{82.41,109.51},{45.73,83.49},{ 9.62,34.83},
  { 3.74,58.22},{11.68,57.95},{32.51,65.83},{77.26,114.26},
  {49.82,84.79},{28.71,54.87},{37.75,72.29},{59.31,106.63},
  {51.90,94.91},{20.46,70.92},{85.50,127.14},{81.14,136.98},
  {55.69,112.07},{48.79,99.85},{24.46,55.17},{26.47,68.48},
  {42.43,87.73},{80.52,114.92},{84.14,140.89},{86.86,141.71},
  {12.68,30.35},{ 3.73,42.17},{ 7.60,60.63},{64.34,126.44},
  {85.86,122.89},{57.23,114.70},{47.53,82.24},{30.72,79.85},
  {52.33,111.71},{30.61,82.25},{97.16,144.89},{30.87,80.86},
  {17.41,52.54},{81.92,121.35},{19.04,51.89},{42.32,75.01},
  {83.73,119.97},{39.70,76.42},{83.70,133.99},{11.67,59.68},
  {77.52,135.54},{65.02,98.96},{ 7.11,51.56},{66.44,108.60},
  {28.34,93.97},{71.65,110.83},{13.83,64.17},{79.87,120.42},
  {89.87,141.39},{79.64,122.06},{51.93,89.38},{26.51,77.05},
  {10.92,38.97},{ 5.97,40.70},{77.61,119.67},{13.14,44.19},
  {87.71,116.88},{57.53,92.75},{27.62,70.09},{29.27,69.03},
  {20.73,65.84},{38.19,60.27},{86.61,124.87},{90.66,135.23},
  {77.13,144.86},{13.31,45.32},{35.55,71.23},{80.84,145.99},
  {93.69,155.92},{ 5.83,50.37},{ 7.94,44.67},{84.78,125.79},
  {11.87,61.17},{35.87,79.17},{48.95,81.77},{42.87,68.65},
  {43.10,86.84},{89.16,143.31},{38.42,87.05},{76.17,111.65},
  {20.53,54.64},{66.33,106.64},{19.73,61.52},{80.44,143.66},
  {46.08,69.80},{17.62,46.53},{41.39,81.66},{76.12,114.60},
  {32.26,65.65},{98.98,121.50},{ 1.96,31.81},{71.69,109.95},
  {46.76,88.26},{38.41,77.51},{35.47,63.13},{62.18,104.82},
  {28.42,67.86},{96.24,138.38},{41.83,64.70},{80.31,149.33},
  {50.88,88.60},{ 1.66,37.83},{99.91,142.80},{57.39,82.67},
  {12.86,46.49},{58.79,90.08},{65.58,109.78},{49.54,88.45},
  { 6.37,41.62},{67.51,108.56},{18.24,50.92},{77.98,131.81},
  {90.20,148.03},{21.38,75.94},{42.88,93.95},{96.38,149.32},
  { 5.07,44.28},{52.75,80.06},{50.70,108.89},{81.88,115.56},
  { 4.60,37.51},{38.17,73.12},{14.99,45.82},{10.97,47.18},
  {75.32,101.50},{95.95,137.43},{50.57,95.91},{ 0.07,55.83},
  {32.42,56.02},{66.81,110.09},{59.14,102.50},{39.88,88.37},
  {57.85,100.14},{52.50,80.54},{22.84,72.37},{38.72,82.42},
  {14.48,47.15},{82.81,125.64},{57.23,95.95},{19.63,63.02},
  {90.48,125.97},{71.53,110.76},{25.76,87.23},{28.43,58.78},
  {98.37,151.15},{55.71,88.79},{74.61,117.52},{20.01,65.00},
  {58.04,88.71},{32.14,66.53},{39.87,82.46},{49.01,94.39},
  {84.59,123.84},{ 3.08,32.69},{50.45,106.16},{26.42,68.12},
  {53.29,86.20},{ 2.72,57.51},{ 1.54,29.53},{35.49,72.18},
  {79.91,138.59},{85.79,129.25},{88.35,154.88},{44.52,78.35},
  {30.21,69.77},{77.29,120.37},{89.62,142.68},{64.90,111.51},
  {98.16,141.77},{15.36,49.63},{ 0.37,37.14},{82.93,132.02},
  {27.58,59.23},{ 6.08,43.11},{89.91,137.00},{20.28,61.45},
  {94.17,138.67},{93.67,124.21},{36.48,94.01},{83.43,119.68},
  {83.33,144.57},{21.92,67.84},{39.66,63.24},{30.35,60.92},
  {46.61,82.91},{27.87,68.14},{82.22,138.02},{86.29,143.66},
  {10.95,57.85},{43.78,89.55},{40.95,99.32},{30.04,71.57},
  {32.84,78.86},{93.70,142.56},{86.15,129.33},{80.62,121.02},
  {53.96,82.26},{58.19,108.04},{67.13,112.76},{16.76,73.94},
  {20.60,67.45},{51.76,90.16},{23.21,66.24},{55.99,102.86},
  {73.70,125.40},{31.94,81.57},{78.51,121.66},{79.90,113.17},
  { 9.68,26.24},{67.92,121.15},{78.62,120.05},{98.45,122.39},
  {57.05,101.16},{96.08,134.21},{41.81,68.90},{92.42,139.92},
  {50.30,83.41},{28.67,69.79},{77.25,127.93},{57.85,103.22},
  {44.48,75.12},{67.49,113.63},{26.51,68.03},{12.33,31.87},
  {29.27,75.12},{81.10,128.69},{72.49,122.98},{ 6.01,41.59},
  {79.87,129.32},{10.51,28.90},{60.30,111.99},{ 1.26,52.98},
  { 4.88,28.91},{ 2.98,46.82},{88.60,147.69},{24.52,54.10},
  {89.48,141.16},{40.27,92.25},{74.78,117.52},{30.04,81.65},
  { 5.71,28.41},{52.20,102.10},{48.34,88.34},{60.97,106.85},
  {63.86,107.83},{28.10,62.35},{89.62,129.03},{38.97,77.26},
  {88.69,155.39},{27.49,62.47},{87.41,132.05},{82.11,134.45},
  {79.18,118.30},{59.62,100.75},{ 8.45,54.09},{ 6.00,61.12},
  { 8.44,49.34},{35.73,81.23},{18.75,34.33},{ 9.78,51.38},
  {44.86,73.93},{78.69,135.02},{11.04,45.15},{24.43,49.65},
  {42.03,58.97},{40.40,82.21},{24.07,86.92},{54.24,113.12},
  {54.77,89.25},{62.12,105.79},{70.63,126.01},{15.77,75.29},
  {39.32,65.92},{84.30,139.09},{92.34,147.50},{40.31,78.05},
  {13.49,50.25},{87.67,138.67},{47.38,97.39},{82.71,133.50},
  {63.75,92.03},{88.34,142.39},{31.86,78.46},{60.26,97.14},
  {16.03,39.72},{91.76,142.42},{78.15,126.26},{ 3.06,37.66},
  {20.55,45.39},{57.54,97.64},{72.00,128.48},{21.09,68.82},
  { 2.45,35.53},{74.77,122.90},{80.88,118.20},{76.70,126.19},
  {72.69,114.68},{ 3.74,39.81},{72.18,113.83},{68.12,126.70},
  {57.45,112.45},{11.52,48.76},{44.65,77.15},{53.14,79.26},
  {75.78,135.05},{ 5.83,31.59},{92.02,140.96},{29.75,69.06},
  {60.58,99.74},{46.53,77.98},{19.95,64.23},{86.68,149.10},
  {45.06,76.86},{51.39,80.24},{80.79,122.30},{73.59,116.53},
  {50.96,100.97},{94.21,153.47},{86.68,124.49},{28.62,65.69},
  { 8.78,42.38},{53.35,90.09},{59.11,97.29},{13.06,50.81},
  {42.57,91.35},{13.41,43.16},{68.72,108.20},{68.55,104.63},
  {62.61,108.58},{85.64,113.26},{98.84,160.36},{ 0.19,36.29},
  {58.07,93.16},{33.71,71.58},{41.97,70.27},{17.98,48.84},
  {96.32,152.67},{21.67,67.96},{19.69,66.15},{45.43,89.15},
  {36.83,70.71},{89.54,149.37},{74.30,121.36},{53.12,111.79},
  { 2.34,35.53},{57.07,119.67},{20.49,57.49},{72.87,128.30},
  {46.63,94.42},{36.77,100.65},{90.66,140.08},{68.92,112.37},
  {82.84,145.28},{56.07,108.27},{12.12,64.91},{94.05,147.84},
  {56.37,84.90},{72.85,120.36},{27.81,64.37},{88.73,151.39},
  {62.88,105.87},{15.31,46.92},{31.82,81.06},{39.65,101.87},
  {84.27,120.37},{88.79,139.21},{14.19,45.44},{65.84,118.07},
  {72.67,109.95},{38.81,76.52},{91.48,139.40},{59.40,115.85},
  {17.04,42.52},{ 1.75,27.23},{83.42,127.38},{50.29,80.97},
  {81.42,123.22},{29.92,76.14},{ 7.17,37.08},{68.79,107.63},
  {50.84,104.49},{87.27,141.04},{32.07,56.88},{99.92,145.67},
  {14.57,53.89},{58.88,91.73},{18.80,37.56},{38.30,76.37},
  {75.07,131.78},{12.57,54.81},{66.46,119.08},{58.35,101.90},
  {29.52,72.98},{31.11,68.93},{95.28,133.59},{50.97,98.95},
  {96.48,124.92},{14.72,59.72},{33.57,73.66},{ 6.46,49.69},
  {89.52,161.37},{40.24,81.66},{98.37,140.08},{58.30,101.01},
  {27.93,62.99},{51.38,89.40},{79.18,124.01},{44.73,86.13},
  {15.53,41.29},{ 9.66,39.98},{54.08,83.34},{44.47,78.96},
  {89.22,124.26},{57.98,107.28},{62.38,112.62},{74.98,125.71},
  {43.69,96.91},{67.33,104.22},{56.90,78.79},{96.01,144.67},
  {45.66,69.55},{93.00,141.30},{77.86,125.94},{49.47,83.31},
  {88.73,138.14},{ 2.74,42.22},{35.73,67.54},{45.61,75.28}
};
