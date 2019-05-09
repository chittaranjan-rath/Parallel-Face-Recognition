# Parallel-Face-Recognition

Data collection source URL: <br>

<i>https://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html</i><br>
<i>https://fei.edu.br/~cet/facedatabase.html</i><br>
<i>https://cswww.essex.ac.uk/mv/allfaces/faces95.html</i><br>
<i>https://cswww.essex.ac.uk/mv/allfaces/faces94.html</i><br>
<i>http://vision.ucsd.edu/~iskwak/ExtYaleDatabase/ExtYaleB.html</i><br>

The above datasets were colored images which were converted to grayscale and then read into a csv file for further processing.<br>

An example dataset can be found in data folder. Other medium to large datasets link can be found in data/dataset_link.md file.<br>

<h3>Requirements:</h3><br>
install boost<br>
	sudo apt install libboost-all-dev


<strong>Navigate to</strong> : 
src/model

<strong>To run Parallel KNN</strong><br>
To compile: g++ -fopenmp -o main main.cpp<br>

<h3>General Execution Format<\h3><br>
./main train_dataset_path test_dataset_path width height distance_measure k1 k2 algorithm

<h3>Distance Measure Notation</h3><br>
Euclidean: 11
Manhattan: 22
Chebychev: 33
Hellinger: 44

<h3>algorithm notation</h3><br>
Kmeans: 0
KNN: 1
Average Face: 2
Ensemble: 3

<h3>k1, k2 notation</h3><br>
k1: k value for KMeans
k2: k value for KNN

To run: KNN on reduced dataset Faces95
./main ../../data/Faces95/ReducedTrainingDataFaces95.csv ../../data/Faces95/ReducedTestFaces95.csv 196 196 11 0 3 1

To run: KMeans on reduced dataset Faces95
./main ../../data/Faces95/ReducedTrainingDataFaces95.csv ../../data/Faces95/ReducedTestFaces95.csv 196 196 11 72 0 0

To run: Average Face on reduced dataset Faces95
./main ../../data/Faces95/ReducedTrainingDataFaces95.csv ../../data/Faces95/ReducedTestFaces95.csv 196 196 11 0 0 2

To run: Ensemble on reduced dataset Faces95
./main ../../data/Faces95/ReducedTrainingDataFaces95.csv ../../data/Faces95/ReducedTestFaces95.csv 196 196 11 72 3 3



-------
<strong>For SVD Implementation</strong>

extract src.zip present inside folder Perform_SVD

Ran on input data and perform dimensionality reduction till 90% contributing features taken cumulatively ( i.e. till 200 columns ) on input.

Final result is saved in <i>PostSVD_NewDimension.ods</i> ( with 200 features ).


------------
Test Data 

Test Data with SVD is provided in "FinalTestData.csv" . Perform test on the same.

Steps to RUN Program : 

1) Change Path
Run : g++ -o Perform_SVD Perform_SVD.cpp
./Perform_SVD

2) Create The CSV File For Reduced Training Data
g++ -o ProcessNewData ProcessNewData.cpp
./ProcessNewData ./ToyDataSet/newData.txt 40 ./ToyDataSet/ReducedTrainingDataToy.csv

3) Create CSV Reduced Testing Data
g++ -o ReduceTestData ReduceTestData.cpp
./ReduceTestData ./ToyDataSet/toy_test.csv 1 ./ToyDataSet/V_Values.txt ./ToyDataSet/ReducedTest.csv 64 41



