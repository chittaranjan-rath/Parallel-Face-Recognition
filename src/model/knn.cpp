#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp>
#include <omp.h>
#include <unistd.h>
#include "create_directories.h"
#include "plotting.h"


using namespace std;

int IMAGES_PER_SUBJECT = 8;
int THREADS  = 7;

typedef unsigned char uchar;

int StrToInt(std::string const& s){

    std::istringstream iss(s);
    int value;
    if (!(iss >> value)) throw std::runtime_error("invalid int");
    return value;
}

bool decending_order(const pair<double,string> &a,const pair<double,string> &b){

       return (a.first > b.first); 
} 

void split_and_transform(vector<vector<int>>& image_pixel,vector<string>& img_info,string input,int i){

    vector<string> result; 

    boost::split(result, input, boost::is_any_of(",")); 

    vector<int> int_result;

    img_info[i] = result[0];

    transform(result.begin()+2, result.end(), back_inserter(int_result), StrToInt);

    image_pixel[i] = int_result;
}

void print_image(vector<int> img,string filename){

    int temp = 0; 
    int width = 92, height = 112;  

    unsigned char *buff = new unsigned char[width*height*sizeof(uchar)];

    ofstream fout (filename.c_str());

    if (!fout.is_open()){

       cout << "Can't open output file"  << filename << endl;
       exit(1);
    }

    for (int i = 0; i < width*height; i++)
      buff[i] = img[i] % 256;
  
    
    fout << "P5\n" << width << " " << height<<"\n" << " 255\n";

    fout.write((char *)buff, width*height*sizeof(uchar));

    fout.close();

    delete[] buff;
}

void read_from_csv(vector<vector<int>>& image,vector<string>& img_info,string filename){

  string line;
  ifstream image_dataset (filename);
  std::vector<string> lines;
  if(image_dataset.is_open()){

    while(getline(image_dataset,line)){
      lines.push_back(line);
      // split_and_transform(image,img_info,line);
      
    }
    image_dataset.close();
  }
  else{
    cout << "Unable to open file";
  }  
  image.resize(lines.size());
  img_info.resize(lines.size());
  #pragma omp parallel for num_threads(THREADS)
  for(int i=0;i<lines.size();i++){
    split_and_transform(image,img_info,lines[i],i);
  }
}

double euclidean_distance(vector<int>image1,vector<int>image2){

  double dist = 0.0;
  // #pragma omp parallel for schedule(static) num_threads(THREADS) 
  #pragma omp parallel for reduction(+:dist) num_threads(THREADS)
  for(int i = 0;i<image1.size();i++){

    // #pragma omp atomic
    dist += pow((image1[i] - image2[i]),2);

  }

  dist = sqrt(dist);

  return dist;
}

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

      distances[j].first = euclidean_distance(test_image[i],train_image[j]);
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

double calculate_accuracy(vector<string>& predicted_image_info,vector<string>& test_image_info){

  double total_size = predicted_image_info.size();
  double correct_prediction = 0.0;
  double accuracy = 0.0;
  // cout<<THREADS<<"\n";
  #pragma omp parallel for reduction(+:correct_prediction) num_threads(THREADS)
  for(int i = 0;i<predicted_image_info.size();i++){

    if(predicted_image_info[i] == test_image_info[i]){
      correct_prediction += 1.0;
    }

  }

  accuracy = correct_prediction/total_size;

  return accuracy;
}

void visulalize_output(vector<vector<int>>& test_images,vector<string>& predicted,vector<string>& actual,string parent_filename){

  string filename = "";
  #pragma omp parallel for firstprivate(filename) num_threads(THREADS)
  for(int i = 0;i<actual.size();i++){
    filename = to_string(i) + actual[i]+" vs "+predicted[i];
    print_image(test_images[i],parent_filename + filename);
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

  // string filename = "cYale/cYale_train_dataset.csv";

  string filename = "cl.cam.ac.uk_facedatabase/train_image_dataset.csv";

  read_from_csv(train_images,train_image_info,filename);  

  // filename = "cYale/cYale_test_dataset.csv";
  filename = "cl.cam.ac.uk_facedatabase/test_image_dataset.csv";  				

  read_from_csv(test_images,test_image_info,filename);  
  
  predicted_image_info.resize(test_images.size());

  //KNN Predction Model

  int k = 2;
  int NUM_TIME = 5;
  double time[NUM_TIME];
  double accuracy_list[NUM_TIME];
  // int threads[NUM_TIME] = {7,8,9,10,11};
  // int threads[NUM_TIME] = {1,2,3,4,5};
  int threads[NUM_TIME] = {5,6,7,8,9};
  // int threads[NUM_TIME] = {1}; 

  for(int i = 0;i<NUM_TIME;i++){

  	THREADS = threads[i];

  	double t = omp_get_wtime();
  	knn_prediction(train_images,test_images,train_image_info,predicted_image_info,k);
	double f = omp_get_wtime();
	
	time[i] = f-t;

	printf("%f\n",f-t);

	accuracy = calculate_accuracy(predicted_image_info,test_image_info);

	accuracy_list[i] = accuracy;

	cout<<"KNN Accuracy: "<<accuracy<<"\n";

	visulalize_output(test_images,predicted_image_info,test_image_info,"prediction_knn/");

  }

  threads_vs_time_plot(threads, time, NUM_TIME);
  threads_vs_time_plot(threads, accuracy_list, NUM_TIME);

  return 0;

}