# Parallel-Face-Recognition

Data collection source URL: https://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html

install boost

sudo apt install libboost-all-dev


Navigate to
src/model

#To run Parallel KNN
To compile: g++ -fopenmp -o knn knn.cpp
To run: ./knn

#To run Serial KNN
export OMP_NUM_THREADS=1
Now follow the same command for running Parallel KNN


#To run Parallel Means
To compile: g++ -fopenmp -o means means.cpp
To run: ./means

#To run Serial Means
export OMP_NUM_THREADS=1
Now follow the same command for running Parallel Means


-------
For SVD Implementation 

extract src.zip present inside folder Perform_SVD

Ran on input data and perform dimensionality reduction till 86% contributing features taken cumulatively ( i.e. till 200 columns ) on input.

Final result is saved in "PostSVD_NewDimension.ods" ( with 200 features ).


