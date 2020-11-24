// Author: Chao-wang Huang
// Date: Monday, January 27, 2003
// Analysis of Jakes Flat Fading Model

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

const double lc = 3.0e8; /* speed of light in m/sec */
const float vc = 250.0; /* speed of vehicle in km/hr */
const double fc = 2.0e9; /* carrier frequency */
const double sr = 25.0e3; /* sampling frequency in sample/sec */
const int ns = 25000; /* sample points */

void histgram(float, int, int, int, int *);
void JakesFading(double, float, double, int, double *);

int main(void)
{
	int i, k, *hist_mag, *hist_ph;
 	double xc, xs, fade[2];
 	double ts, power, fd, mag, phase, *pdf_mag, *pdf_ph;
 	time_t t, start, end;
 	FILE *description, *mag_pdf, *ph_pdf, *magnitude, *ph;

   start = time(NULL);
	printf("Program is running. Don't close, please!\n");

 	description = fopen("description.log", "w");
   mag_pdf = fopen("mag_pdf.log", "w");
   ph_pdf = fopen("ph_pdf.log", "w");
   magnitude = fopen("amplitude.log", "w");
   ph = fopen("phase.log", "w");

   hist_mag=(int *)malloc(400*sizeof(int));
   hist_ph=(int *)malloc(800*sizeof(int));
   pdf_mag=(double *)malloc(400*sizeof(double));
   pdf_ph=(double *)malloc(800*sizeof(double));

   for(i=0; i<400; i++)
      *(hist_mag+i) = 0;
   for(i=0; i<800; i++)
      *(hist_ph+i) = 0;

	srand((unsigned) time(&t));

 	fd=(vc*1000.0/3600.0)/(lc/fc);  /* doppler frequency */

 	fprintf(description,"The velocity of car in km/hr = %.4e\n",vc);
 	fprintf(description,"Doppler frequency in Hz= %.4e\n",fd);
 	fprintf(description,"Sampling rate in sample/sec = %.4e\n",sr);
 	fprintf(description,"The number of samples = %d\n",ns);

 	power=0.0;
   ts = 15.0;
 	for(k=0; k<ns; k++)
  	{
      JakesFading(fc, vc*1000/3600.0, ts, 2, &fade[0]);
      ts += (1/sr);
      xc = fade[0];
      xs = fade[1];
    	power += (xc*xc+xs*xs)/(float)ns;
      mag = sqrt(pow(xc,2)+pow(xs,2));
      phase = atan2(xs,xc);

      if(k<25000)
      {
			fprintf(magnitude, "%f\n", mag);
         fprintf(ph, "%f\n", phase);
      }

// Histgram of amplitude and phase
      histgram(mag, 0, 4, 400, hist_mag);
      histgram(phase, -4, 4, 800, hist_ph);
   }

   fprintf(description,"average power = %.4e\n",power);

   for(i=0; i<400; i++)
   	*(pdf_mag+i) = (*(hist_mag+i)/(float)ns)/0.01;
   for(i=0; i<800; i++)
      *(pdf_ph+i) = (*(hist_ph+i)/(float)ns)/0.01;
   for(i=0; i<400; i++)
   	fprintf(mag_pdf, "%f\n", *(pdf_mag+i));
   for(i=0; i<800; i++)
   	fprintf(ph_pdf, "%f\n", *(pdf_ph+i));

   fclose(description);
   fclose(mag_pdf);
   fclose(ph_pdf);
   fclose(magnitude);
   fclose(ph);
   free(hist_mag);
   free(hist_ph);
   free(pdf_mag);
   free(pdf_ph);

   printf("This program is ended. Press any key to continue.\n");
   end = time(NULL);
   printf("Totle elapsed time: %.0f(sec)", difftime(end,start));
   getch();

   return 0;
 }

void histgram(float in, int left, int right, int section, int *statistic)
{
	int i;

   if(in<left || in>=right)
   	printf("Warning!! The input number is out of range.\n");
   else
   {
   	for(i=0; i<section; i++)
   		if(in>=left+((right-left)/(float)section)*i && in<left+((right-left)/(float)section)*(i+1))
         {
      		*(statistic+i) = *(statistic+i) + 1;
            break;
         }
   }
}

void JakesFading(double f_c/*Hz*/, float v/*m/s*/, double t/*s*/, int type, double *fade)
{
	const double C = 3.0e8;     // (m/s)
   const float Pi = 3.14159265358979;
   int n, N, N_o = 32;
   double lamda, w_m, beta_n, w_n, alpha, T_c2, T_s2, theta_n;

   lamda = C/f_c;     // wave length (meter)
   w_m = 2.0*Pi*v/lamda;    // maximum Doppler frequency
   N = 2*(2*N_o+1);

   switch(type)
   {
   	case 1:
   		alpha = 0.0;
         T_c2 = (double)N_o;
         T_s2 = (double)N_o + 1.0;
         break;
      case 2:
      	alpha = 0.0;
         T_c2 = (double)N_o + 1.0;
         T_s2 = (double)N_o;
         break;
      case 3:
      	alpha = Pi/4.0;
         T_c2 = (double)N_o + 0.5;
         T_s2 = (double)N_o + 0.5;
         break;
      default:
      	printf("\nInvalid type selection for Jake's fading channel model.\n");
         break;
   }

   if(v == 0.0)
   {
   	*(fade+0) = 1.0;
      *(fade+1) = 0.0;
   }
   else
   {
   	*(fade+0) = sqrt(1.0/T_c2)*cos(alpha)*cos(w_m*t);
      *(fade+1) = sqrt(1.0/T_s2)*sin(alpha)*cos(w_m*t);

      for(n = 1; n <= N_o; n++)
      {
      	switch(type)
         {
         	case 1:
            	beta_n = (double)n*Pi/((double)N_o+1.0);
               break;
            case 2:
            	beta_n = (double)n*Pi/(double)N_o;
               break;
            case 3:
            	beta_n = (double)n*Pi/(double)N_o;
               break;
         	default:
            	break;
         }
         w_n = w_m*cos(2.0*Pi*(double)n/(double)N);
//            theta_n = 2.0*Pi*((double)rand()/(double)RAND_MAX);  // random phase
			theta_n = 0.0;
         *(fade+0) += sqrt(2.0/T_c2)*cos(beta_n)*cos(w_n*t+theta_n);
         *(fade+1) += sqrt(2.0/T_s2)*sin(beta_n)*cos(w_n*t+theta_n);
		}
	}
}

