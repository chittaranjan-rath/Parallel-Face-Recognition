#ifndef FILEOPEN_H
#define FILEOPEN_H

#include <sys/stat.h>
#include<stdio.h>

using namespace std;


void create_output_directories(){
    
    int dir_err = mkdir("avg_face", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        printf("Error creating directory avg_face !n");
    }
    dir_err = mkdir("avg_face_parallel", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        printf("Error creating directory avg_face_parallel !n");
    }
    dir_err = mkdir("prediction_knn", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        printf("Error creating directory prediction_knn !n");
    }
    dir_err = mkdir("prediction_mean", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        printf("Error creating directory prediction_mean !n");
    }


}

#endif
