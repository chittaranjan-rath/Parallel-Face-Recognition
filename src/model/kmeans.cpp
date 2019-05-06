
#include "header_files.h"

int THREADS = 4;
// #define IMAGES_PER_SUBJECT 8
int width = 92;
int height = 112;

using namespace std;


void printClusterNames(vector<string> cluster_names){
	cout<<"cluster_names are: ";
	for(int i=0;i<cluster_names.size();i++){
		cout<<cluster_names[i]<<" ";
	}
	cout<<endl;
}
void random_initialize_clusters(vector<vector<int>> &cluster_centers,vector<vector<int>> &image_pixel){
	srand(time(0)); 
	for(int i=0; i<cluster_centers.size();i++){
		cluster_centers[i] = image_pixel[i*IMAGES_PER_SUBJECT];
		// for(int j=0;j<cluster_centers[i].size();j++){
		// 	cluster_centers[i][j] = abs(rand())%256;
		// }
	}
}
double findDistance(vector<int> clustercenter,vector<int> image){
	// cout<<"clustercenter.size()="<<clustercenter.size()<<" "<<" image size = "<<image.size()<<endl;
	double dist = 0.0;
	for(int i=0;i<clustercenter.size();i++){
		double temp = (clustercenter[i] - image[i]) ;
		dist += temp * temp; 
	}
	return dist;
}
int findMinDistanceToClusterIndex(vector<vector<int>> &cluster_centers,vector<int> image){
	double mindist =INT_MAX*1.0;
	int retindex = 0;
	for(int i=0;i<cluster_centers.size();i++){
		double temp = findDistance(cluster_centers[i],image);
		if(temp<mindist){
			mindist = temp;
			retindex = i;
		}
	}
	return retindex;
}
void assignClusterName(vector<vector<pair<vector<int>,string>>> &clustered_points,vector<string> &cluster_names,int k){
	cluster_names.clear();
	for(int i=0;i<clustered_points.size();i++){
		if( clustered_points[i].size()>0){
			set<string> distinct_img_info;
			map<string,int> temp;
			for(int j=0;j<clustered_points[i].size();j++){
				// cout<<"b4 inserting into set "<<clustered_points[i][j].second<<endl;
				distinct_img_info.insert(clustered_points[i][j].second);
			}
			// cout<<"after insertion "<<endl;
			for(auto it=distinct_img_info.begin(); it!=distinct_img_info.end(); it++){
				// cout<<(*it)<<endl;
				temp.insert({*it,0});
				for(int j=0;j<clustered_points[i].size();j++){
					if(*it==clustered_points[i][j].second)
						temp[*it] += 1;
				}	
			}
			int maxv = 0;
			string ans ="";
			for(auto it = temp.begin();it!=temp.end();it++){
				if(it->second>maxv){
					maxv = it->second;
					ans  = it->first;
					cout<<"ans = "<<ans;
				}
			}
			cluster_names.push_back(ans);
			temp.clear();
		}
		else
			cluster_names.push_back("NO NAME");
		
	}

}
void clustering(vector<vector<pair<vector<int>,string>>> &clustered_points,vector<vector<int>> &cluster_centers,vector<vector<int>> &image_pixel, vector<string> &img_info){
	
	for(int i=0;i<image_pixel.size();i++){
		int clusterindex = findMinDistanceToClusterIndex(cluster_centers,image_pixel[i]);
		// cout<<"clusterindex = "<<clusterindex<<" img_info[i] "<<img_info[i]<<endl;
		clustered_points[clusterindex].push_back({image_pixel[i],img_info[i]});
		// cout<<"inserted "<<clustered_points[clusterindex][clustered_points[clusterindex].size()-1].second<<" size of clusterd pts[clusterindex] = "<<clustered_points[clusterindex].size()<<endl;
	}
	// cout<<"image_pixel size = "<<image_pixel.size()<<endl;
	// cout<<"clustered points size"<<clustered_points.size()<<endl;
	for(int i=0;i<clustered_points.size() ;i++){
		if( clustered_points[i].size()>0){
			// cout<<" i = "<<i<<" "<<clustered_points[i].size()<<" a"<<endl;
			// cout<<clustered_points[i][0].second<<" b"<<endl;
		}
	}
	cout<<"--XXXX---"<<endl;
	
}
double calc_error(vector<string> &cluster_names,vector<vector<pair<vector<int>,string>>> clustered_points,vector<vector<int>> &image_pixel, vector<string> &img_info){
	cout<<"in clacError cluster_names are: ";
	for(int i=0;i<cluster_names.size();i++){
		cout<<cluster_names[i]<<" ";
	}
	cout<<endl;
	int cnt = 0;int tot = 0;
	for(int i=0; i<clustered_points.size();i++){
		cout<<cluster_names[0]<<endl;
		for(int j=0; j<clustered_points[i].size(); j++){
			if(cluster_names[i]==clustered_points[i][j].second)
				cnt += 1;
			cout<<"i= "<<i<<" cluster_names[i] "<<cluster_names[i]<<" clustered_points[i][j].second "<<clustered_points[i][j].second<<endl;
			tot += 1;
		}
	}
	cout<<"cnt = "<<cnt<<" tot = "<<tot<<endl;
	if(cnt==0)
		return 1.0;
	return ((tot-cnt)*1.0)/(tot*1.0);
}
void update_cluster_centers(vector<vector<int>> &cluster_centers,vector<vector<pair<vector<int>,string>>> clustered_points,int k){
	vector<vector<int>> centres_copy = cluster_centers;
	cluster_centers.clear();
	for(int i=0; i<clustered_points.size();i++){
		cout<<"here"<<endl;
		
		if(clustered_points[i].size()>0){
			vector<int> temp(clustered_points[i][0].first.size(),0);
			for(int j=0; j<clustered_points[i].size(); j++){
				for(int k=0;k<clustered_points[i][j].first.size();k++){
					temp[k] += clustered_points[i][j].first[k];
				}
			}
		
			for(int k=0; k<temp.size(); k++){
				temp[k] /= clustered_points[i].size();
			}
			cluster_centers.push_back(temp);
		}
		else{
			cluster_centers.push_back(centres_copy[i]);
		}
	}
}
void kmeans(vector<vector<int>> &image_pixel, vector<string> &img_info,int k){
	int num_subjects = int(img_info.size() / IMAGES_PER_SUBJECT);
	int x = image_pixel.size();
	
	vector<vector<int>> cluster_centers(k,vector<int>(image_pixel[0].size()));
	random_initialize_clusters(cluster_centers,image_pixel);
	double error = 1.0;
	double threshold = 0.00;
	int limit=5;int c_limit=0;
	bool convergence = false;
	while(!convergence){
		c_limit++;
		vector<vector<pair<vector<int>,string>>> clustered_points(k,vector<pair<vector<int>,string>>());
		vector<string> cluster_names(k);
		clustering(clustered_points,cluster_centers,image_pixel,img_info);
		cout<<"A"<<endl;
		assignClusterName(clustered_points,cluster_names,k);
		cout<<"B"<<endl;
		printClusterNames(cluster_names);
		error = calc_error(cluster_names,clustered_points,image_pixel,img_info);
		// printClusterNames(cluster_names);
		cout<<"error = "<< error <<endl;
		update_cluster_centers(cluster_centers,clustered_points,k);
		if(error<threshold || limit==c_limit){
			for(int i=0; i<cluster_centers.size(); i++){
				print_image(cluster_centers[i],to_string(i+1),width,height);
			}
			
			cout<<"accuracy = "<<1-error<<endl;
			convergence = true;
		}
		
	}
	// for(int i=0; i<cluster_centers.size();i++){
	// 	for(int j=0;j<cluster_centers[i].size();j++){
	// 		cout<<cluster_centers[i][j]<<" " ;
	// 	}
	// 	cout<<endl;
	// }
}
int main (int argv, char **argc) {

  cout<<argv<<endl;
  string s1(argc[1]);
  string s2(argc[2]);
  cout<<s1<<" "<<s2<<endl;
  if((int)argv!=3){
    cout<<"not enough parameters .... ex ./a.out width height"<<endl;
  }
  else{
      width = stoi(s1);
      height  = stoi(s2);
  }
  

	double t = omp_get_wtime();

	vector<vector<int>> avg_face, avg_face_parallel;
	vector<vector<int>> train_images;
	vector<string> avg_face_info;
	vector<string> train_image_info;

	vector<vector<int>> test_images;
	vector<string> test_image_info;
	vector<string> predicted_image_info;

	double accuracy = 0.0;

	// string filename = "../../data/train_image_dataset.csv";
	// string filename = "../../data/Faces95/ReducedTrainingDataFaces95.csv";
  	string filename = "../../data/Faces95/faces95_train_dataset.csv";



	
	read_from_csv(train_images, train_image_info, filename,THREADS,1);

	// filename = "../../data/test_image_dataset.csv";
	 // filename = "../../data/Faces95/ReducedTestFaces95.csv";
  	filename = "../../data/Faces95/faces95_test_dataset.csv";			

	read_from_csv(test_images, test_image_info, filename,THREADS,0);

	predicted_image_info.resize(test_images.size());

	int k = 40;
	// cout<<train_images.size()<<"  "<<train_image_info.size()<<endl;
	// cout<<train_images[0].size()<<endl;
	// for (int i=0;i<train_images[0].size();i++)
	// 	cout<<train_images[0][i]<<" ";
	// cout<<endl<<endl<<endl;
	// for (int i=0;i<train_image_info.size();i++)
	// 	cout<<train_image_info[i]<<" ";
	
	kmeans(train_images,train_image_info,k);
	
	double f = omp_get_wtime();

	printf("%f\n", f - t);

	return 0;
}