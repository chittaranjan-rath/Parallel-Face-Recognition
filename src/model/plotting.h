#ifndef PLOT_H
#define PLOT_H

#include <stdio.h>

using namespace std;


void threads_vs_time_plot(int threads[], double time_list[],int n){

	int i = 0;
	for(i = 0;i<n;i++){
		
		printf("%f\n",time_list[i]);
	}

	double nth[n];

	for(i = 0;i<n;i++){

		nth[i] = (double)threads[i];

		printf("%d\n",threads[i]);
	}

	
	char * commandsForGnuplot[] = {"set title \"Thread v/s Time\"", "set xlabel 'Threads'\n" 
	,"set ylabel 'Time'\n","set style data lines\n","plot \"data.temp\" lw 8 linecolor rgb \"blue\"\n" };

    
    FILE * temp = fopen("data.temp", "w");
    FILE * gnuplotPipe = popen("gnuplot -persistent", "w");
    

    
    for (i=0; i < n; i++)
    {
    	fprintf(temp, "%lf %lf \n", nth[i],time_list[i]); //Write the data to a temporary file
    	// cout<<"Here\n";
    }

    // cout<<"Here2\n";

    for (i=0; i < n; i++)
    {
    	fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    	// cout<<"Here\n";
    }

    // sleep(5);

    // pclose(gnuplotPipe);
    // fclose(temp);


}

#endif
