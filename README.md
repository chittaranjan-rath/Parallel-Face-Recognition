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
