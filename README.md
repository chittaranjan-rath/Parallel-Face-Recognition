# Parallel-Face-Recognition

Data collection source URL: <br>

<i>https://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html</i><br>
<i>https://fei.edu.br/~cet/facedatabase.html</i><br>

install boost<br>
sudo apt install libboost-all-dev


<strong>Navigate to</strong> : 
src/model

<strong>To run Parallel KNN</strong><br>
To compile: g++ -fopenmp -o knn knn.cpp<br>
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

Ran on input data and perform dimensionality reduction till 99% contributing features taken cumulatively ( i.e. till 200 columns ) on input.

Final result is saved in <i>PostSVD_NewDimension.ods</i> ( with 200 features ).


------------
Test Data 

Test Data with SVD is provided in "FinalTestData.csv" . Perform test on the same.  


