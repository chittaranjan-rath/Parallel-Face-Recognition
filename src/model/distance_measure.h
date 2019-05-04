#ifndef DISTANCE_H
#define DISTANCE_H


using namespace std;


double manhattan_distance(vector<int>image1,vector<int>image2,int threads){

  double dist = 0.0;
  // #pragma omp parallel for schedule(static) num_threads(threads) 
  #pragma omp parallel for reduction(+:dist) num_threads(threads)
  for(int i = 0;i<image1.size();i++){

    // #pragma omp atomic
    dist += abs(image1[i] - image2[i]);

  }

  return dist;
}

double euclidean_distance(vector<int>image1,vector<int>image2,int threads){

  double dist = 0.0;
  // #pragma omp parallel for schedule(static) num_threads(threads) 
  #pragma omp parallel for reduction(+:dist) num_threads(threads)
  for(int i = 0;i<image1.size();i++){

    // #pragma omp atomic
    dist += pow((image1[i] - image2[i]),2);

  }

  dist = sqrt(dist);

  return dist;
}



#endif
