#ifndef FILE_H
#define FILE_H

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
    // cout<<"input "<<input<<endl;
    vector<string> result; 

    boost::split(result, input, boost::is_any_of(",")); 
    // cout<<"result size "<<result.size()<<endl;
    // cout<<"last elem "<<result[result.size()-1]<<endl;
    // cout<<"lastbut 1 elem "<<result[result.size()-2]<<endl;

    vector<int> int_result;

    img_info[i] = result[0];
    
    if(result[result.size()-1].length() == 0 )
    transform(result.begin()+2, result.end()-1, back_inserter(int_result), StrToInt);
    else
    transform(result.begin()+2, result.end(), back_inserter(int_result), StrToInt);
    image_pixel[i] = int_result;
}

void read_from_csv(vector<vector<int>>& image,vector<string>& img_info,string filename,int threads){

  string line;
  ifstream image_dataset (filename);
  std::vector<string> lines;
  int lines_read = 0;
  if(image_dataset.is_open()){

    while(getline(image_dataset,line)){
      lines.push_back(line);
      lines_read++;
      
      // split_and_transform(image,img_info,line);
      
    }
    image_dataset.close();
  }
  else{
    cout << "Unable to open file";
  }  
  // cout<<"line count "<<lines_read<<endl;
  image.resize(lines.size());
  // cout<<"b4 line 57 "<<endl;
  img_info.resize(lines.size());
  // cout<<"b4 line 60 "<<endl;
  #pragma omp parallel for num_threads(threads)
  for(int i=0;i<lines.size();i++){
    // cout<<"inside for  "<<endl;
    split_and_transform(image,img_info,lines[i],i);
  }
  cout<<"file_hnadling over "<<endl;
}

#endif