#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp>
#include <omp.h>
#include <unistd.h>
#define THREADS 4
#define IMAGES_PER_SUBJECT 8

using namespace std;


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
    #pragma omp parallel for 
    for(int i=0;i<num_subjects;i++){

        avg_face_info[i] = img_info[IMAGES_PER_SUBJECT*i];
      
        start_index[i] = IMAGES_PER_SUBJECT * i;
        end_index[i] = IMAGES_PER_SUBJECT * (i+1) - 1 ;
      
        for(int j= start_index[i];j<=end_index[i]; j++){
      
          for (int k=0;k<image_pixel[0].size();k++)
      
            avg_face[i][k] += image_pixel[j][k] ; 
      
        }
    }
  // }
    #pragma omp parallel for
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
  #pragma omp parallel for
  for(int i=0;i<lines.size();i++){
    split_and_transform(image,img_info,lines[i],i);
  }
}

double euclidean_distance(vector<int>image1,vector<int>image2){

  double dist = 0.0;
  // #pragma omp parallel for schedule(static) num_threads(THREADS) 
  #pragma omp parallel for reduction(+:dist)
  for(int i = 0;i<image1.size();i++){

    // #pragma omp atomic
    dist += pow((image1[i] - image2[i]),2);

  }

  dist = sqrt(dist);

  return dist;
}

void means_prediction(vector<vector<int>>& avg_face,vector<vector<int>>& test_images,vector<string>& avg_face_info,vector<string>& predicted_image_info){

  vector<double> min_distance(test_images.size());
  

  #pragma omp parallel for 
  for(int i = 0;i<test_images.size();i++){
    double temp;
    min_distance[i] = 0.0;

    // #pragma omp parallel for schedule(static) num_threads(THREADS)
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

void knn_prediction(vector<vector<int>>& train_image,vector<vector<int>>& test_image,vector<string>& train_image_info,vector<string>& predicted_image_info,int k){

  double min_distance = 0.0,temp;
  int num_subjects = train_image.size()/IMAGES_PER_SUBJECT;


  for(int i = 0;i<test_image.size();i++){

    vector<pair<double,string>> distances(train_image.size());
    vector<int> vote(num_subjects,0);
    
    for(int j = 0;j<train_image.size();j++){

      distances[j].first = euclidean_distance(test_image[i],train_image[j]);
      distances[j].second = train_image_info[j];
    }

    sort(distances.begin(),distances.end());

    for(int j=0;j<k;j++){
      
      string temp = distances[j].second;

      temp.erase(temp.begin());

      int index = stoi(temp)-1;

      vote[index] += 1;

    }

    int max_vote = INT_MIN;
    string prediction = "";



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

  #pragma omp parallel for reduction(+:correct_prediction)
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
  #pragma omp parallel for firstprivate(filename)
  for(int i = 0;i<actual.size();i++){
    filename = actual[i]+" vs "+predicted[i];
    print_image(test_images[i],parent_filename + filename);
  }
}

int main () {


  double t = omp_get_wtime();

  
  vector<vector<int>> avg_face,avg_face_parallel;
  vector<vector<int>> train_images;
  vector<string> avg_face_info;
  vector<string> train_image_info;

  vector<vector<int>> test_images;
  vector<string> test_image_info;
  vector<string> predicted_image_info;

  double accuracy = 0.0;

  string filename = "../../data/train_image_dataset.csv";

  read_from_csv(train_images,train_image_info,filename);  

  filename = "../../data/test_image_dataset.csv";

  read_from_csv(test_images,test_image_info,filename);  
  
  predicted_image_info.resize(test_images.size());

  //Means Prediction Model

//   avg_face = parallel_average_face(train_images,train_image_info,avg_face_info);

//   avg_face = generate_all_avg_face(train_images,train_image_info,avg_face_info);

//   means_prediction(avg_face,test_images,avg_face_info,predicted_image_info);
  
//   visulalize_output(test_images,predicted_image_info,test_image_info,"prediction_mean/");

//   accuracy = calculate_accuracy(predicted_image_info,test_image_info);

//   cout<<"Mean face accuracy: "<<accuracy<<"\n";

  //KNN Predction Model

  int k = 1;

  knn_prediction(train_images,test_images,train_image_info,predicted_image_info,k);

  accuracy = calculate_accuracy(predicted_image_info,test_image_info);

  cout<<"KNN Accuracy: "<<accuracy<<"\n";

  visulalize_output(test_images,predicted_image_info,test_image_info,"prediction_knn/");

  double f = omp_get_wtime();

  printf("%f\n",f-t);

  return 0;

}