#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp>
#include <omp.h>
#include <unistd.h>
#include "create_directories.h"
#include "plotting.h"

using namespace std;

int IMAGES_PER_SUBJECT = 40;
int THREADS  = 1;

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
    // int width = 92, height = 112;
    int width = 168, height = 192;

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

vector<vector<int>> parallel_average_face(vector<vector<int>>& image_pixel,vector<string>& img_info,vector<string>& avg_face_info){
  
  int num_subjects = int(img_info.size()/IMAGES_PER_SUBJECT);

  vector<int> start_index(num_subjects); 
  vector<int> end_index(num_subjects); 

  vector<vector<int>> avg_face;

  avg_face.resize(num_subjects,vector<int>(image_pixel[0].size()));
  avg_face_info.resize(num_subjects);
  
  string str = "avg_face_parallel/";
  // #pragma omp parallel
  // {
    // #pragma omp nowait
    #pragma omp parallel for num_threads(THREADS)
    for(int i=0;i<num_subjects;i++){

        avg_face_info[i] = img_info[IMAGES_PER_SUBJECT*i];
      
        start_index[i] = IMAGES_PER_SUBJECT * i;
        end_index[i] = IMAGES_PER_SUBJECT * (i+1) - 1 ;
      	
        #pragma omp collaspe(2) nowait
        for(int j= start_index[i];j<=end_index[i]; j++){
      
          for (int k=0;k<image_pixel[0].size();k++)
      
            avg_face[i][k] += image_pixel[j][k] ; 
      
        }
    }

    #pragma omp parallel for num_threads(THREADS)
    for(int i=0;i<avg_face.size();i++){
      
      for (int k=0;k<avg_face[0].size();k++)
        avg_face[i][k] /= IMAGES_PER_SUBJECT;

      print_image(avg_face[i],str+to_string(i));
    }
  
  return avg_face;
}

vector<int> average_face(vector<vector<int>>& image_pixel,vector<string>& img_info,string subject_id){

  vector<int> avg_face;
  int count = 0;

  for(int i = 0;i<image_pixel.size();i++){

    if(img_info[i] == subject_id){

      if(avg_face.size()==0){
        count++;
        avg_face = image_pixel[i];
      }else{
        count++;
        for(int j = 0;j<image_pixel[i].size();j++){
          avg_face[j] += image_pixel[i][j];
        }
      }

    }

  }

  for(int i = 0;i<avg_face.size();i++){
    avg_face[i] = (int)(avg_face[i]/count);
  }
  
  return avg_face;
}

vector<vector<int>> generate_all_avg_face(vector<vector<int>>& image_pixel,vector<string>& img_info,vector<string>& avg_face_info){

  vector<vector<int>> avg_face;
  string filename = "";
  string str = "avg_face/";
  string subject_id = "";
  int count = 0;



  for(int i = 0;i<image_pixel.size();i++){

    if(subject_id != img_info[i]){

        avg_face_info.push_back(img_info[i]);

        filename = img_info[i];
        // print_image(image_pixel[i],filename);
        
        subject_id = img_info[i]; 
        // parallel_average_face(image_pixel,img_info,img_info[i]);
        avg_face.push_back(average_face(image_pixel,img_info,img_info[i]));
        
        filename = img_info[i]+"avg";

        print_image(avg_face[count],str + filename);
        
        count++;

    }
  }
  return avg_face;
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

void means_prediction(vector<vector<int>>& avg_face,vector<vector<int>>& test_images,vector<string>& avg_face_info,vector<string>& predicted_image_info){

  vector<double> min_distance(test_images.size());
  

  #pragma omp parallel for num_threads(THREADS)
  for(int i = 0;i<test_images.size();i++){
  	cout<<"processing test image: "<<i<<"\n";
    double temp;
    min_distance[i] = 0.0;

    #pragma omp parallel for schedule(static) num_threads(THREADS)
    for(int j = 0;j<avg_face.size();j++){

      if(j==0){
        min_distance[i] = euclidean_distance(avg_face[j],test_images[i]);
        predicted_image_info[i] = avg_face_info[j];
      }else{

        temp = euclidean_distance(avg_face[j],test_images[i]);

        if(temp<min_distance[i]){

          min_distance[i] = temp;
          predicted_image_info[i] = avg_face_info[j];
        }

      }

    }

  } 
}

double calculate_accuracy(vector<string>& predicted_image_info,vector<string>& test_image_info){

  double total_size = predicted_image_info.size();
  double correct_prediction = 0.0;
  double accuracy = 0.0;

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

  // string filename = "cl.cam.ac.uk_facedatabase/train_image_dataset.csv";
  string filename = "cYale/cYale_train_dataset.csv";
  // string filename = "f95/faces95_train_dataset.csv";
  // string filename = "f94/faces94_train_dataset.csv";

  

  read_from_csv(train_images,train_image_info,filename);  

  // filename = "cl.cam.ac.uk_facedatabase/test_image_dataset.csv";
  filename = "cYale/cYale_test_dataset.csv";
  // filename = "f94/faces94_test_dataset.csv";
  // filename = "f95/faces95_test_dataset.csv";

  read_from_csv(test_images,test_image_info,filename);  
  
  predicted_image_info.resize(test_images.size());

  //Means Prediction Model

  int k = 1;
  int NUM_TIME = 5;
  double time[NUM_TIME];
  double accuracy_list[NUM_TIME];
  int threads[5] = {1,2,4,8,16}; 

  for(int i = 0;i<NUM_TIME;i++){

    THREADS = threads[i];

    double t = omp_get_wtime();

    avg_face = parallel_average_face(train_images,train_image_info,avg_face_info);
    // avg_face = generate_all_avg_face(train_images,train_image_info,avg_face_info);
    means_prediction(avg_face,test_images,avg_face_info,predicted_image_info);
    
    double f = omp_get_wtime();

    time[i] = f-t;

    visulalize_output(test_images,predicted_image_info,test_image_info,"prediction_mean/");

    accuracy = calculate_accuracy(predicted_image_info,test_image_info);

    accuracy_list[i] = accuracy;

    cout<<"Mean face accuracy: "<<accuracy<<"\n";
    
    printf("%f\n",f-t);

  }

  threads_vs_time_plot(threads, time, NUM_TIME);
  // threads_vs_time_plot(threads, accuracy_list, NUM_TIME);

  return 0;

}