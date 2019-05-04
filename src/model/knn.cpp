#include "header_files.h"

using namespace std;

int IMAGES_PER_SUBJECT = 8;
int THREADS  = 7;
int width = 92;
int height = 112;


void knn_prediction(vector<vector<int>>& train_image,vector<vector<int>>& test_image,vector<string>& train_image_info,vector<string>& predicted_image_info,int k){

  double min_distance = 0.0,temp;
  int num_subjects = train_image.size()/IMAGES_PER_SUBJECT;

  #pragma omp parallel for num_threads(THREADS)
  for(int i = 0;i<test_image.size();i++){
  	cout<<"Processing test image "<<i<<"\n";
    vector<pair<double,string>> distances(train_image.size());
    vector<int> vote(num_subjects,0);
    
    #pragma omp parallel for num_threads(THREADS)
    for(int j = 0;j<train_image.size();j++){

      distances[j].first = manhattan_distance(test_image[i],train_image[j],THREADS);
      // distances[j].first = euclidean_distance(test_image[i],train_image[j],THREADS);
      distances[j].second = train_image_info[j];
    }

    sort(distances.begin(),distances.end());

    #pragma omp parallel for num_threads(THREADS)
    for(int j=0;j<k;j++){
      
      string temp = distances[j].second;

      temp.erase(temp.begin());

      int index = stoi(temp)-1;

      vote[index] += 1;

    }

    int max_vote = INT_MIN;
    string prediction = "";


    #pragma omp parallel for num_threads(THREADS)
    for(int j = 0;j<vote.size();j++){

      if(vote[j]>max_vote){
        max_vote = vote[j];
        prediction = "S"+to_string(j+1);
      }
    }

    predicted_image_info[i] = prediction;

  }  
}


int main () {

  create_output_directories();

  vector<vector<int>> avg_face,avg_face_parallel;
  vector<vector<int>> train_images;
  vector<string> avg_face_info;
  vector<string> train_image_info;

  vector<vector<int>> test_images;
  vector<string> test_image_info;
  vector<string> predicted_image_info;

  double accuracy = 0.0;
  double precision = 0.0;
  double recall = 0.0;
  double f1 = 0.0;

  // string filename = "cYale/cYale_train_dataset.csv";
  // string filename = "f95/faces95_train_dataset.csv";
  // string filename = "f94/faces94_train_dataset.csv";
  string filename = "cl.cam.ac.uk_facedatabase/train_image_dataset.csv";

  read_from_csv(train_images,train_image_info,filename,THREADS);  

  // filename = "cYale/cYale_test_dataset.csv";
  // filename = "f95/faces95_test_dataset.csv";
  // filename = "f94/faces94_test_dataset.csv";
  filename = "cl.cam.ac.uk_facedatabase/test_image_dataset.csv";  				

  read_from_csv(test_images,test_image_info,filename,THREADS);  
  
  predicted_image_info.resize(test_images.size());

  //KNN Predction Model

  int k = 2;
  int NUM_TIME = 1;
  double time[NUM_TIME];
  double accuracy_list[NUM_TIME];
  double precision_list[NUM_TIME];
  double recall_list[NUM_TIME];
  double f1_score_list[NUM_TIME];
  // int threads[NUM_TIME] = {7,8,9,10,11};
  // int threads[NUM_TIME] = {1,2,3,4,5};
  int threads[NUM_TIME] = {5,2,3,4,6,7,8,9,12,24};
  // int threads[NUM_TIME] = {1}; 

  for(int i = 0;i<NUM_TIME;i++){

    THREADS = threads[i];

    double t = omp_get_wtime();
    knn_prediction(train_images,test_images,train_image_info,predicted_image_info,k);
    double f = omp_get_wtime();

    time[i] = f-t;

    printf("%f\n",f-t);

    accuracy = accuracy_score(predicted_image_info,test_image_info,THREADS);
    accuracy_list[i] = accuracy;

    recall = recall_score(predicted_image_info,test_image_info,THREADS);
    recall_list[i] = recall;

    precision = precision_score(predicted_image_info,test_image_info,THREADS);
    precision_list[i] = precision;

    f1 = f1_score(predicted_image_info,test_image_info,THREADS);
    f1_score_list[i] = f1;

    cout<<"KNN Accuracy: "<<accuracy<<"\n";
    cout<<"KNN Recall: "<<recall<<"\n";
    cout<<"KNN Precision: "<<precision<<"\n";
    cout<<"KNN F1 Score: "<<f1<<"\n";
    cout<<"Confusion Matrix: \n";
    contigency_matrix(predicted_image_info,test_image_info,THREADS,true);
    

    visulalize_output(test_images,predicted_image_info,test_image_info,"prediction_knn/",width,height,THREADS);

  }

  threads_vs_time_plot(threads, time, NUM_TIME);
  threads_vs_time_plot(threads, accuracy_list, NUM_TIME);

  return 0;

}