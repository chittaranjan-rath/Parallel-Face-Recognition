#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp>
#include <omp.h>
#define THREADS 4

using namespace std;


typedef unsigned char uchar;

int StrToInt(std::string const& s){

    std::istringstream iss(s);
    int value;
    if (!(iss >> value)) throw std::runtime_error("invalid int");
    return value;
}

void split_and_transform(vector<vector<int>>& image_pixel,vector<vector<string>>& img_info,string input){

    vector<string> result; 

    boost::split(result, input, boost::is_any_of(",")); 

    vector<int> int_result;

    vector<string> temp;

    temp.push_back(result[0]);
    temp.push_back(result[1]);

    img_info.push_back(temp);

    transform(result.begin()+2, result.end(), back_inserter(int_result), StrToInt);

    image_pixel.push_back(int_result);
}

void print_image(vector<int> img,string filename){

    int temp = 0; 
    int width = 92, height = 112;  
  
    // string filename = "hello.pgm";

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

vector<int> average_face(vector<vector<int>>& image_pixel,vector<vector<string>>& img_info,string subject_id){

  vector<int> avg_face;
  int count = 0;

  for(int i = 0;i<image_pixel.size();i++){

    if(img_info[i][0] == subject_id){

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

vector<vector<int>> generate_all_avg_face(vector<vector<int>>& image_pixel,vector<vector<string>>& img_info,vector<string>& avg_face_info){

  vector<vector<int>> avg_face;
  string filename = "";
  string subject_id = "";
  int count = 0;

  for(int i = 0;i<image_pixel.size();i++){

    if(subject_id != img_info[i][0]){

        avg_face_info.push_back(img_info[i][0]);

        filename = img_info[i][0];
        print_image(image_pixel[i],filename);
        
        subject_id = img_info[i][0];        
        avg_face.push_back(average_face(image_pixel,img_info,img_info[i][0]));
        
        filename = img_info[i][0]+"avg";
        // print_image(avg_face[count],filename);

        count++;

    }
  }
  return avg_face;
}

void read_from_csv(vector<vector<int>>& image,vector<vector<string>>& img_info,string filename){

  string line;
  ifstream image_dataset (filename);

  if(image_dataset.is_open()){

    while(getline(image_dataset,line)){

      split_and_transform(image,img_info,line);
      
    }
    image_dataset.close();
  }
  else{
    cout << "Unable to open file";
  }  
}

double euclidean_distance(vector<int>image1,vector<int>image2){

  double dist = 0.0;
  // #pragma omp parallel for schedule(static) num_threads(THREADS) 
  for(int i = 0;i<image1.size();i++){

    // #pragma omp atomic
    dist += pow((image1[i] - image2[i]),2);

  }

  dist = sqrt(dist);

  return dist;
}

void predict(vector<vector<int>>& avg_face,vector<vector<int>>& test_images,vector<string>& avg_face_info,vector<string>& predicted_image_info){

  double min_distance = 0.0,temp;

  for(int i = 0;i<test_images.size();i++){
  
    min_distance = 0.0;

    // #pragma omp parallel for schedule(static) num_threads(THREADS)
    for(int j = 0;j<avg_face.size();j++){

      if(j==0){
        min_distance = euclidean_distance(avg_face[j],test_images[i]);
        predicted_image_info[i] = avg_face_info[j];
      }else{

        temp = euclidean_distance(avg_face[j],test_images[i]);

        if(temp<min_distance){

          min_distance = temp;
          predicted_image_info[i] = avg_face_info[j];
        }

      }

    }

  }
    
}

double calculate_accuracy(vector<string>& predicted_image_info,vector<vector<string>>& test_image_info){

  double total_size = predicted_image_info.size();
  double correct_prediction = 0.0;
  double accuracy = 0.0;

  for(int i = 0;i<predicted_image_info.size();i++){

    if(predicted_image_info[i] == test_image_info[i][0]){
      correct_prediction += 1.0;
    }

  }

  accuracy = correct_prediction/total_size;

  return accuracy;
}

void visulalize_output(vector<vector<int>>& test_images,vector<string>& predicted,vector<vector<string>>& actual){

  string filename = "";
  for(int i = 0;i<actual.size();i++){
    filename = actual[i][0]+" vs "+predicted[i];
    print_image(test_images[i],filename);
  }

}

int main () {


double t = omp_get_wtime();

// #pragma omp parallel for schedule(dynamic) num_threads(THREADS)
// #pragma omp parallel for schedule(static) num_threads(THREADS)
// #pragma omp parallel for schedule(guided) num_threads(THREADS)
// #pragma omp parallel for schedule(dynamic,100) num_threads(THREADS)

  
  vector<vector<int>> avg_face;
  vector<vector<int>> train_images;
  vector<string> avg_face_info;
  vector<vector<string>> train_image_info;

  vector<vector<int>> test_images;
  vector<vector<string>> test_image_info;
  vector<string> predicted_image_info;

  double accuracy = 0.0;

  string filename = "train_image_dataset.csv";

  read_from_csv(train_images,train_image_info,filename);  

  avg_face = generate_all_avg_face(train_images,train_image_info,avg_face_info);

  filename = "test_image_dataset.csv";

  read_from_csv(test_images,test_image_info,filename);  
  
  predicted_image_info.resize(test_images.size());

  predict(avg_face,test_images,avg_face_info,predicted_image_info);


  accuracy = calculate_accuracy(predicted_image_info,test_image_info);

  cout<<"Accuracy: "<<accuracy<<"\n";

  visulalize_output(test_images,predicted_image_info,test_image_info);

  double f = omp_get_wtime();

  printf("%f\n",f-t);

  return 0;

}