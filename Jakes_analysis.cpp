// Author: Chao-wang Huang
// Date: Tuesday, January 28, 2003
// Analysis of Jakes Flat Fading Model

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

const float lc = 3.0e8; /* speed of light in m/sec */
const float vc = 120.0; /* speed of car in km/hr */
const float fc = 2.0e9; /* carrier frequency */
const float sr = 25.0e3; /* sampling frequency in sample/sec */
const int ns = 1000000; /* sample points */
const float sigma = 1/sqrt(2.0);
const float R_rms = sqrt(2.0)*sigma;
const float T = ns/sr;  /* (second) */

int main(void)
{
   int i, r, lcr, tau;
 	float *fading_real, *fading_image, *record, diff, R, *mag, rate;
   float cov[289], mean1, mean2, mean3, afd;
 	FILE *fade_real, *fade_image, *covariance;

	printf("Program is running. Don't close, please!\n");

   record = (float *)malloc(ns*sizeof(float));
   mag = (float *)malloc(ns*sizeof(float));
   fading_real=(float *)malloc(ns*sizeof(float));
   fading_image=(float *)malloc(ns*sizeof(float));

   covariance = fopen("autocovariance.log", "w");

   fade_real = fopen("fading_real.log", "r");
   for(i=0; i<ns; i++)
		fscanf(fade_real, "%f", (fading_real+i));

	fade_image = fopen("fading_image.log", "r");
	for(i=0; i<ns; i++)
		fscanf(fade_image, "%f", (fading_image+i));

	for(i=0; i<ns; i++)
   	*(mag+i) = sqrt(pow(*(fading_real+i),2)+pow(*(fading_image+i),2));

	for(r=-20; r<=5; r++)
   {
      lcr = 0;
      afd = 0.0;
//   	R = R_rms*pow(10,(r/20.0));
      for(i=0; i<ns; i++)
      {
      	if(20*log10(*(mag+i))>=r)
//      	if(*(mag+i) >= R)
         	*(record+i) = 1.0;
         else
         	*(record+i) = -1.0;
      }

      for(i=1; i<ns; i++)
      {
      	diff = (*(record+i)-*(record+(i-1)))/2.0;
         if(diff>=0.0)
         	lcr++;
      }
      rate = lcr/T;
//      printf("Level crossing rate: %f\n", rate);
//      getch();
		for(i=1; i<ns; i++)
      	if(*(record+i)<0.0)
         	afd += -*(record+i);
      afd = afd/(float)lcr/sr;
//      printf("Average fade duration: %f(sec)\n", afd);
//      getch();
   }

   mean1 = 0.0;
   for(i=0; i<(ns-288); i++)
   	mean1 += *(mag+i)/(float)(ns-288);

   for(tau=0; tau<=288; tau++)
   {
   	mean2 = mean3 = 0.0;
   	for(i=0; i<(ns-288); i++)
   	{
         mean2 += *(mag+i+tau)/(float)(ns-288);
      	mean3 += *(mag+i)*(*(mag+i+tau))/(float)(ns-288);
      }
      cov[tau] = mean3 - mean1*mean2;
      fprintf(covariance, "%f\n",cov[tau]);
   }

   mean1 = mean2 = mean3 = 0.0;
   for(i=0; i<ns; i++)
   {
   	mean1 += pow(*(fading_real+i),2)/(float)ns;
      mean2 += pow(*(fading_image+i),2)/(float)ns;
      mean3 += *(fading_real+i)*(*(fading_image+i))/(float)ns;
   }
   printf("%f %f %f\n", mean1, mean2, mean3);
   getch();

   free(fading_real);
   free(fading_image);
   free(record);
   free(mag);
   fclose(fade_real);
   fclose(fade_image);
   fclose(covariance);

   printf("This program is ended. Press any key to continue.\n");
   getch();

   return 0;
}

