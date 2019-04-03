#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp>

using namespace std;

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

int main () {

  string line;
  ifstream image_dataset ("image_dataset.csv");

  vector<vector<int>> image_pixel;
  vector<vector<string>> img_info;

  if(image_dataset.is_open()){

    while(getline(image_dataset,line)){

      split_and_transform(image_pixel,img_info,line);
      
    }
    image_dataset.close();
  }
  else{
    cout << "Unable to open file";
  }  

  for(int i = 0;i<img_info.size();i++){

    cout<<img_info[i][0]<<" "<<img_info[i][1]<<"\n";

  }

  // for(int i = 0;i<image_pixel.size();i++){
  //   cout<<image_pixel[i].size()<<"\n\n";
  //   for(int j = 0;j<image_pixel[i].size();j++){
  //     cout<<image_pixel[i][j]<<" ";
  //   }
  //   cout<<"\n";
  //   break;
  // }

  return 0;

}