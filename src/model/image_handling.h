#ifndef IMAGE_H
#define IMAGE_H


using namespace std;


void print_image(vector<int> img,string filename,int width, int height){

    int temp = 0; 

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


void visulalize_output(vector<vector<int>>& test_images,vector<string>& predicted,vector<string>& actual,string parent_filename,
																							int width,int height,int threads){

  string filename = "";
  #pragma omp parallel for firstprivate(filename) num_threads(threads)
  for(int i = 0;i<actual.size();i++){
    filename = to_string(i) + actual[i]+" vs "+predicted[i];
    print_image(test_images[i],parent_filename + filename,width,height);
  }
}


#endif


