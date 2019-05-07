#ifndef ENSEMBLE_H
#define ENSEMBLE_H

vector<string> ApplyEnsemble(vector<string> predicted_KMeans,vector<string> predicted_KNN,vector<string> average_face,int _size)
{
	vector<string> final_prediction_Col(_size);
	
	#pragma omp parellel for 
	for(int k=0;k<_size;k++)
	{
		string final_prediction = "";
		map<string,int> _value;
		string kMeans = predicted_KMeans[k];
		string KnnPredicted = predicted_KNN[k];
		string average_face_predicted = average_face[k];
		
		_value[kMeans] += 1;
		_value[KnnPredicted] += 1;
		_value[average_face_predicted] += 1;

		map<string,int>::iterator itr=_value.begin();

		for(;itr!=_value.end();itr++)
		{
			if(itr->second >= 2)
			{
				final_prediction = itr->first;
				break;
			}
		}

		if(final_prediction == "")
		{
			final_prediction = KnnPredicted;
		}

		final_prediction_Col[k] = final_prediction;
		final_prediction = "";
		_value.clear();
	}
	return final_prediction_Col;
}

/*
int main()
{

	vector<vector<int> > avg_face, avg_face_parallel;
        vector<string> avg_face_info;


	int k = k_knn;
        int NUM_TIME = 11;
        double time[NUM_TIME];
        double accuracy_list[NUM_TIME];
        double precision_list[NUM_TIME];
        double recall_list[NUM_TIME];
        double f1_score_list[NUM_TIME];
        int threads[11] = {1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};

        string temp_Name = "Original";
        std::size_t found = filename_train.find("Reduced");
        if (found != std::string::npos)
            temp_Name = "Reduced";


        ostringstream str2;
        str2 << k_kmeans;
        string sNewMEANS = str2.str();


        ostringstream str1;
        str1 << k_knn;
        string sNewKNN = str1.str();

        ofstream myfile;
        string to_open = "ensemble_knn_" + sNewKNN +"_KMeans_"+ sNewMEANS + "_" + temp_Name + "_" + code_dist_m(distance_measure) + ".csv";
        myfile.open(to_open.c_str());

        // myfile.open("knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure).c_str()+".csv");
        myfile << "dataset_name,Threads,Time,Distance_measure,Accuracy,Recall,Precision,F1score"
               << "\n";

        for (int i = 0; i < NUM_TIME; i++)
        {

            THREADS = threads[i];


            vector<string> predicted_KMeans;
	    vector<string> predicted_KNN;
	    vector<string> average_face_prediction;
           
            knn_prediction(train_images, test_images, train_image_info, predicted_KNN, k, distance_measure, THREADS);

	    avg_face = parallel_average_face(train_images, train_image_info, avg_face_info, THREADS, width, height);
            // avg_face = generate_all_avg_face(train_images,train_image_info,avg_face_info);
            means_prediction(avg_face, test_images, avg_face_info, average_face_prediction, THREADS, distance_measure);
            
	    kmeans(train_images, train_image_info, k_kmeans, width, height, distance_measure, THREADS, predicted_KMeans, test_images, test_image_info);

	    int _size = predicted_KMeans.size();
	    vector<string> predicted_image_info = ApplyEnsemble(predicted_KMeans,predicted_KNN,average_face_prediction,_size);
           
            accuracy = accuracy_score(predicted_image_info, test_image_info, THREADS);
            accuracy_list[i] = accuracy;

            recall = recall_score(predicted_image_info, test_image_info, THREADS);
            recall_list[i] = recall;

            precision = precision_score(predicted_image_info, test_image_info, THREADS);
            precision_list[i] = precision;

            f1 = f1_score(predicted_image_info, test_image_info, THREADS);
            f1_score_list[i] = f1;

            cout << "KNN Accuracy: " << accuracy << "\n";
            cout << "KNN Recall: " << recall << "\n";
            cout << "KNN Precision: " << precision << "\n";
            cout << "KNN F1 Score: " << f1 << "\n";
            cout << "Confusion Matrix: \n";

            ostringstream str2;
            str2 << time[i];
            string sNew = str2.str();
            myfile << k << ",";
            myfile << filename_train << ",";
            myfile << THREADS << ",";
            myfile << sNew << ",";
            myfile << code_dist_m(distance_measure).c_str() << ",";
            myfile << accuracy << ",";
            myfile << recall << ",";
            myfile << precision << ",";
            myfile << f1 << "\n";

            // contigency_matrix(predicted_image_info, test_image_info, THREADS, true);

            // visulalize_output(test_images, predicted_image_info, test_image_info, "prediction_knn/", width, height, THREADS);
        }
        myfile.close();
}
*/

#endif
