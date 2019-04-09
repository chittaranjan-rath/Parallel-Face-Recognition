# Parallel-Face-Recognition

Data collection source URL: https://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html

install boost

sudo apt install libboost-all-dev


<strong>Navigate to</strong> : 
src/model

<strong>To run Parallel KNN</strong><br>
To compile: g++ -fopenmp -o knn knn.cpp
To run: ./knn

<strong>To run Serial KNN</strong><br>
export OMP_NUM_THREADS=1<br>
Now follow the same command for running Parallel KNN


<strong>To run Parallel Means</strong><br>
To compile: g++ -fopenmp -o means means.cpp<br>
To run: ./means

<strong>To run Serial Means</strong><br>
export OMP_NUM_THREADS=1<br>
Now follow the same command for running Parallel Means


-------
<strong>For SVD Implementation</strong>

extract src.zip present inside folder Perform_SVD

Ran on input data and perform dimensionality reduction till 86% contributing features taken cumulatively ( i.e. till 200 columns ) on input.

Final result is saved in <i>PostSVD_NewDimension.ods</i> ( with 200 features ).


