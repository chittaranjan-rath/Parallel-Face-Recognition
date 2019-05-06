#include "header_files.h"

using namespace std;


int THREADS  = 7;
int width = 92;
int height = 112;


void knn_prediction(vector<vector<int>>& train_image,vector<vector<int>>& test_image,vector<string>& train_image_info,
                                            vector<string>& predicted_image_info,int k){

  double min_distance = 0.0,temp;
  int num_subjects = train_image.size()/IMAGES_PER_SUBJECT;

  #pragma omp parallel for num_threads(THREADS)
  for(int i = 0;i<test_image.size();i++){
  	cout<<"Processing test image "<<i<<"\n";
    vector<pair<double,string>> distances(train_image.size());
    vector<int> vote(num_subjects,0);
    
    #pragma omp parallel for num_threads(THREADS)
    for(int j = 0;j<train_image.size();j++){


      distances[j].first = euclidean_distance(test_image[i],train_image[j],THREADS);
      // distances[j].first = manhattan_distance(test_image[i],train_image[j],THREADS);
      // distances[j].first = chebyshev_distance(test_image[i],train_image[j],THREADS);
      // distances[j].first = hellinger_distance(test_image[i],train_image[j],THREADS);

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


int main (int argv, char **argc) {

  cout<<argv<<endl;
  string s1(argc[1]); // train dataset
  string s2(argc[2]); // test datasret
  string s3(argc[3]); //widtht
  string s4(argc[4]); //heigjt
  string s5(argc[5]); //distance critereia name
  string s6(argc[6]); // k kmeans
  string s7(argc[7]); // k knn
  
  cout<<s1<<" "<<s2<<" "<<s3<<" "<<s4<<" "<<s5<<" "<<s6<<" "<<s7<<endl;
  if((int)argv!=8){
    cout<<"not enough parameters .... ex ./a.out width height"<<endl;
  }
  else{
      width = stoi(s1);
      height  = stoi(s2);
  }
  
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

  
  // string filename = "../../data/Faces95/ReducedTrainingDataFaces95.csv";
  string filename = "../../data/Faces95/faces95_train_dataset.csv";

  
// string filename = "../../data/cYale_train_dataset/ReducedDataSet_Training.csv";
  // string filename = "../../data/cYale_train_dataset/cYale_train_dataset.csv";
  
  cout<<"read now"<<endl;
  read_from_csv(train_images,train_image_info,filename,THREADS,1);  
  cout<<"read 4 train"<<endl;
  
  // filename = "../../data/Faces95/ReducedTestFaces95.csv";
  filename = "../../data/Faces95/faces95_test_dataset.csv";				

  //  filename = "../../data/cYale_train_dataset/ReducedTestData.csv";
  // filename = "../../data/cYale_train_dataset/cYale_test_dataset.csv";				

  read_from_csv(test_images,test_image_info,filename,THREADS,0);  
  cout<<"test_image_info "<<test_image_info.size()<<endl;
  cout<<"train_image_info "<<train_image_info.size()<<endl;
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
  // int threads[NUM_TIME] = {1,2,3,4,6,7,8,9,12,13,14,15,16,17,18,19};
  int threads[NUM_TIME] = {1}; 

  for(int i = 0;i<NUM_TIME;i++){

    THREADS = threads[i];

    double t = omp_get_wtime();
    knn_prediction(train_images,test_images,train_image_info,predicted_image_info,k);
    double f = omp_get_wtime();

    time[i] = f-t;
    ofstream myfile;
    myfile.open ("knn_op.csv");
    myfile << THREADS<<","<<to_string(time[i])<<"\n";
    myfile.close();
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