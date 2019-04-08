# Parallel-Face-Recognition

Data collection source URL: https://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html

install boost

sudo apt install libboost-all-dev


navigate to
src/data_preprocessing
g++ create_directories.cpp

then
g++ -fopenmp means_parallel.cpp

./a.out


-------
For SVD Implementation 

extract src.zip present inside folder Perform_SVD

Ran on input data and perform dimensionality reduction till 86% contributing features taken cumulatively ( i.e. till 200 columns ) on input.

Final result is saved in "PostSVD_NewDimension.ods" ( with 200 features ).


