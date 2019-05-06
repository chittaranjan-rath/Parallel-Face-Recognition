#include "header_files.h"

using namespace std;

int IMAGES_PER_SUBJECT = 8;
int THREADS  = 1;
int width = 92;
int height = 112;

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

      print_image(avg_face[i],str+to_string(i),width,height);
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

        print_image(avg_face[count],str + filename,width,height);
        
        count++;

    }
  }
  return avg_face;
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
        
        // min_distance[i] = euclidean_distance(avg_face[j],test_images[i],THREADS);
        // min_distance[i] = manhattan_distance(avg_face[j],test_images[i],THREADS);
        // min_distance[i] = chebyshev_distance(avg_face[j],test_images[i],THREADS);
        min_distance[i] = hellinger_distance(avg_face[j],test_images[i],THREADS);

        predicted_image_info[i] = avg_face_info[j];
      }else{

        // temp = euclidean_distance(avg_face[j],test_images[i],THREADS);
        // temp = manhattan_distance(avg_face[j],test_images[i],THREADS);
        // temp = chebyshev_distance(avg_face[j],test_images[i],THREADS);
        temp = hellinger_distance(avg_face[j],test_images[i],THREADS);


        if(temp<min_distance[i]){

          min_distance[i] = temp;
          predicted_image_info[i] = avg_face_info[j];
        }

      }

    }

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

  string filename = "cl.cam.ac.uk_facedatabase/train_image_dataset.csv";
  // string filename = "cYale/cYale_train_dataset.csv";
  // string filename = "f95/faces95_train_dataset.csv";
  // string filename = "f94/faces94_train_dataset.csv";

  

  read_from_csv(train_images,train_image_info,filename,THREADS,1);  

  filename = "cl.cam.ac.uk_facedatabase/test_image_dataset.csv";
  // filename = "cYale/cYale_test_dataset.csv";
  // filename = "f94/faces94_test_dataset.csv";
  // filename = "f95/faces95_test_dataset.csv";

  read_from_csv(test_images,test_image_info,filename,THREADS,0);  
  
  predicted_image_info.resize(test_images.size());

  //Means Prediction Model

  int k = 1;
  int NUM_TIME = 1;
  double time[NUM_TIME];
  
  double accuracy_list[NUM_TIME];
  double precision_list[NUM_TIME];
  double recall_list[NUM_TIME];
  double f1_score_list[NUM_TIME];

  int threads[5] = {5,2,4,8,16}; 

  for(int i = 0;i<NUM_TIME;i++){

    THREADS = threads[i];

    double t = omp_get_wtime();

    avg_face = parallel_average_face(train_images,train_image_info,avg_face_info);
    // avg_face = generate_all_avg_face(train_images,train_image_info,avg_face_info);
    means_prediction(avg_face,test_images,avg_face_info,predicted_image_info);
    
    double f = omp_get_wtime();
    printf("%f\n",f-t);
    time[i] = f-t;


    accuracy = accuracy_score(predicted_image_info,test_image_info,THREADS);
    accuracy_list[i] = accuracy;

    recall = recall_score(predicted_image_info,test_image_info,THREADS);
    recall_list[i] = recall;

    precision = precision_score(predicted_image_info,test_image_info,THREADS);
    precision_list[i] = precision;

    f1 = f1_score(predicted_image_info,test_image_info,THREADS);
    f1_score_list[i] = f1;

    cout<<"Mean face Accuracy: "<<accuracy<<"\n";
    cout<<"Mean face Recall: "<<recall<<"\n";
    cout<<"Mean face Precision: "<<precision<<"\n";
    cout<<"Mean face F1 Score: "<<f1<<"\n";
    cout<<"Confusion Matrix: \n";
    contigency_matrix(predicted_image_info,test_image_info,THREADS,true);

    visulalize_output(test_images,predicted_image_info,test_image_info,"prediction_knn/",width,height,THREADS);

  }

  threads_vs_time_plot(threads, time, NUM_TIME);
  // threads_vs_time_plot(threads, accuracy_list, NUM_TIME);

  return 0;

}