#ifndef KMEANS_H
#define KMEANS_H

// using namespace std;


vector<vector<int>> parallel_average_face(vector<vector<int>>& image_pixel,vector<string>& img_info,vector<string>& avg_face_info,int THREADS){
  
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

    //   print_image(avg_face[i],str+to_string(i),width,height);
    }
  
  return avg_face;
}


void printClusterNames(vector<string> cluster_names)
{
    cout << "cluster_names are: ";
    for (int i = 0; i < cluster_names.size(); i++)
    {
        cout << cluster_names[i] << " ";
    }
    cout << endl;
}
void random_initialize_clusters(vector<vector<int>> &cluster_centers, vector<vector<int>> &image_pixel,vector<string> &img_info,int THREADS)
{
    vector<string> avg_face_info;
    vector<vector<int>>  avg_face = parallel_average_face(image_pixel,img_info,avg_face_info,THREADS);
    srand(time(0));
    // for(int tempi=0; tempi<avg_face[0].size(); tempi++)
            // cout<<"S1: "<<avg_face[0][0]<<" ";
    #pragma omp parallel for num_threads(THREADS)
    for (int i = 0; i < cluster_centers.size(); i++)
    {
        cluster_centers[i] = avg_face[i];
        
        // cluster_centers[i] = image_pixel[i * IMAGES_PER_SUBJECT];
        // for(int j=0;j<cluster_centers[i].size();j++){
        // 	cluster_centers[i][j] = abs(rand())%256;
        // }
    }
}
/*double findDistance(vector<int> clustercenter, vector<int> image)
{
    // cout<<"clustercenter.size()="<<clustercenter.size()<<" "<<" image size = "<<image.size()<<endl;
    double dist = 0.0;
    for (int i = 0; i < clustercenter.size(); i++)
    {
        double temp = (clustercenter[i] - image[i]);
        dist += temp * temp;
    }
    return dist;
}*/
int findMinDistanceToClusterIndex(vector<vector<int>> &cluster_centers, vector<int> image, int dist_criteria, int THREADS)
{
    double mindist = INT_MAX * 1.0;
    int retindex = 0;
    //  #pragma omp parallel for reduction(min:mindist) num_threads(threads)

    for (int i = 0; i < cluster_centers.size(); i++)
    {

        double temp;
        if (dist_criteria == euc)
            temp = euclidean_distance(cluster_centers[i], image, THREADS);
        else if (dist_criteria == man)
            temp = manhattan_distance(cluster_centers[i], image, THREADS);
        else if (dist_criteria == che)
            temp = chebyshev_distance(cluster_centers[i], image, THREADS);
        else if (dist_criteria == hel)
            temp = hellinger_distance(cluster_centers[i], image, THREADS);

        // double temp = findDistance(cluster_centers[i], image);
        if (temp < mindist)
        {
            mindist = temp;
            retindex = i;
        }
    }
    return retindex;
}
void assignClusterName(vector<vector<pair<vector<int>, string>>> &clustered_points, vector<string> &cluster_names, int k,int THREADS)
{
    // cluster_names.clear();
    // cout<<cluster_names.size()<<" cluste sizesssssssss"<<endl;
    #pragma omp parallel for num_threads(THREADS)
    for (int i = 0; i < clustered_points.size(); i++)
    {
        if (clustered_points[i].size() > 0)
        {
            set<string> distinct_img_info;
            map<string, int> temp;
            for (int j = 0; j < clustered_points[i].size(); j++)
            {
                // cout<<"b4 inserting into set "<<clustered_points[i][j].second<<endl;
                distinct_img_info.insert(clustered_points[i][j].second);
            }
            // cout<<"after insertion "<<endl;
            for (auto it = distinct_img_info.begin(); it != distinct_img_info.end(); it++)
            {
                // cout<<(*it)<<endl;
                temp.insert({*it, 0});
                for (int j = 0; j < clustered_points[i].size(); j++)
                {
                    if (*it == clustered_points[i][j].second)
                        temp[*it] += 1;
                }
            }
            int maxv = 0;
            string ans = "";
            for (auto it = temp.begin(); it != temp.end(); it++)
            {
                if (it->second > maxv)
                {
                    maxv = it->second;
                    ans = it->first;
                    // cout << "ans = " << ans;
                }
            }
            // cluster_names.push_back(ans);
            cluster_names[i] = ans;
            temp.clear();
        }
        else
            cluster_names[i] = "NO NAME";
            // cluster_names.push_back("NO NAME");
    }
}
void clustering(vector<vector<pair<vector<int>, string>>> &clustered_points, vector<vector<int>> &cluster_centers, vector<vector<int>> &image_pixel, vector<string> &img_info, int dist_m, int THREADS)
{
    // #pragma omp parallel for num_threads(THREADS)
    for (int i = 0; i < image_pixel.size(); i++)
    {
        
        int clusterindex = findMinDistanceToClusterIndex(cluster_centers, image_pixel[i], dist_m, THREADS);
        // cout<<"clusterindex = "<<clusterindex<<" img_info[i] "<<img_info[i]<<endl;
        clustered_points[clusterindex].push_back({image_pixel[i], img_info[i]});
        // cout<<"inserted "<<clustered_points[clusterindex][clustered_points[clusterindex].size()-1].second<<" size of clusterd pts[clusterindex] = "<<clustered_points[clusterindex].size()<<endl;
    }
   
}
double calc_error(vector<string> &cluster_names, vector<vector<pair<vector<int>, string>>> clustered_points, vector<vector<int>> &image_pixel, vector<string> &img_info,int threads)
{
    // cout << "in clacError cluster_names are: ";
    // for (int i = 0; i < cluster_names.size(); i++)
    // {
    //     cout << cluster_names[i] << " ";
    // }
    // cout << endl;
    int cnt = 0;
    int tot = 0;
    for (int i = 0; i < clustered_points.size(); i++)
    {
        // cout << cluster_names[0] << endl;
        #pragma omp parallel for reduction(+:cnt) reduction(+:tot) num_threads(threads)
        for (int j = 0; j < clustered_points[i].size(); j++)
        {
            if (cluster_names[i] == clustered_points[i][j].second)
                cnt += 1;
            // cout << "i= " << i << " cluster_names[i] " << cluster_names[i] << " clustered_points[i][j].second " << clustered_points[i][j].second << endl;
            tot += 1;
        }
    }
    // cout << "cnt = " << cnt << " tot = " << tot << endl;
    if (cnt == 0)
        return 1.0;
    return ((tot - cnt) * 1.0) / (tot * 1.0);
}
void update_cluster_centers(vector<vector<int>> &cluster_centers, vector<vector<pair<vector<int>, string>>> clustered_points, int k,int THREADS)
{
    // cout<<"in update cluster "<<endl;
    vector<vector<int>> centres_copy = cluster_centers;
    cluster_centers.clear();
    
    for (int i = 0; i < clustered_points.size(); i++)
    {
        // cout << "here" << endl;

        if (clustered_points[i].size() > 0)
        {
            vector<int> temp(clustered_points[i][0].first.size(), 0);
            // #pragma omp parallel for num_threads(THREADS)
            for (int j = 0; j < clustered_points[i].size(); j++)
            {
                //#pragma omp parallel for num_threads(THREADS)
                for (int k = 0; k < clustered_points[i][j].first.size(); k++)
                {
                    temp[k] += clustered_points[i][j].first[k];
                }
            }
            //#pragma omp parallel for num_threads(THREADS)
            for (int k = 0; k < temp.size(); k++)
            {
                temp[k] /= clustered_points[i].size();
            }
            cluster_centers.push_back(temp);
        }
        else
        {
            cluster_centers.push_back(centres_copy[i]);
        }
    }
    // cout<<" update cluster ENded "<<endl;
}
void kmeans(vector<vector<int>> &image_pixel, vector<string> &img_info, int k, int width, int height, int dist_m, int THREADS,vector<string>& predicted_image_info,vector<vector<int>> test_images,vector<string> test_image_info)
{
    int num_subjects = int(img_info.size() / IMAGES_PER_SUBJECT);
    int x = image_pixel.size();
    vector<vector<int>> cluster_centers(k, vector<int>(image_pixel[0].size()));
    random_initialize_clusters(cluster_centers, image_pixel,img_info,THREADS);
    double error = 1.0;
    double threshold = 0.00;
    int limit = 5;
    int c_limit = 0;
    bool convergence = false;
    vector<string> cluster_names_copy(k);
    while (!convergence)
    {

        c_limit++;
        vector<vector<pair<vector<int>, string>>> clustered_points(k, vector<pair<vector<int>, string>>());
        vector<string> cluster_names(k," ");
        // cout<<"clustering b4"<<endl;
        clustering(clustered_points, cluster_centers, image_pixel, img_info, dist_m, THREADS);
        // cout<<"clustering done"<<endl;
        assignClusterName(clustered_points, cluster_names, k, THREADS);
        // cout<<"assignClusterName done"<<endl;
        // printClusterNames(cluster_names);
        error = calc_error(cluster_names, clustered_points, image_pixel, img_info, THREADS);
        // cout<<"calc_error done"<<endl;
        // printClusterNames(cluster_names);
        // cout << "error (in kmeans)= " << error << endl;
        update_cluster_centers(cluster_centers, clustered_points, k, THREADS);
        // cout<<"update_cluster_centers done"<<endl;
        cluster_names_copy = cluster_names;
        if (error < threshold || limit == c_limit)
        {
            // for (int i = 0; i < cluster_centers.size(); i++)
            // {
            //     ostringstream str2;
            //     str2 << (i + 1);
            //     string sNew = str2.str();
            //     // print_image(cluster_centers[i], sNew, width, height);
            // }
            cout << "accuracy = " << 1 - error << endl;
            convergence = true;
        }
        // cout<<"loop iteration done"<<endl;
    }
    
    // cout<<"testing started"<<endl;
    #pragma omp parallel for num_threads(THREADS)
    for(int i=0; i<test_images.size();i++){
        int retval = findMinDistanceToClusterIndex(cluster_centers, test_images[i], dist_m, THREADS);
        predicted_image_info[i] = cluster_names_copy[retval];
    }
    // cout<<"testing doone return to main.cpp"<<endl;
}

#endif