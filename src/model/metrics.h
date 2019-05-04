#ifndef METRICS_H
#define METRICS_H

using namespace std;


vector<vector<int>> contigency_matrix(vector<string>& predicted_image_info,vector<string>& test_image_info,int threads,bool to_print=true){

  double total_size = predicted_image_info.size();

  	vector<string> unique_classes;

  	
	for(int i = 0;i<test_image_info.size();i++){

		if(find(unique_classes.begin(), unique_classes.end(), test_image_info[i]) == unique_classes.end()){
			unique_classes.push_back(test_image_info[i]);
		} 
	}  
  
  
	vector<vector<int>>confusion_matrix(unique_classes.size(),vector<int>(unique_classes.size(),0));
	
	int col_index, row_index;
	vector<string>::iterator it;

	for(int i = 0;i<test_image_info.size();i++){

		it = find(unique_classes.begin(), unique_classes.end(), test_image_info[i]); 
		if (it != unique_classes.end()){
			row_index =  it - unique_classes.begin(); 
		}

		it = find(unique_classes.begin(), unique_classes.end(), predicted_image_info[i]); 
		if (it != unique_classes.end()){
			col_index =  it - unique_classes.begin(); 
		}

		confusion_matrix[row_index][col_index] += 1;

	}

	if(to_print){

		for(int i = 0;i<unique_classes.size();i++)
			cout<<unique_classes[i]<<" ";
		cout<<"\n";

		for(int i = 0;i<confusion_matrix.size();i++){
			cout<<unique_classes[i]<<" ";
			for(int j = 0;j<confusion_matrix[0].size();j++){
				cout<<confusion_matrix[i][j]<<" ";
			}
			cout<<"\n";
		}

	}

	return confusion_matrix;
}


double accuracy_score(vector<string>& predicted_image_info,vector<string>& test_image_info,int threads){

  double total_size = predicted_image_info.size();
  double correct_prediction = 0.0;
  double accuracy = 0.0;
  // cout<<threads<<"\n";
  #pragma omp parallel for reduction(+:correct_prediction) num_threads(threads)
  for(int i = 0;i<predicted_image_info.size();i++){

    if(predicted_image_info[i] == test_image_info[i]){
      correct_prediction += 1.0;
    }

  }

  accuracy = correct_prediction/total_size;

  return accuracy;
}

double precision_score(vector<string>& predicted_image_info,vector<string>& test_image_info,int threads){

	vector<vector<int>> confusion_matrix;
	confusion_matrix = contigency_matrix(predicted_image_info,test_image_info,threads,false);
	vector<double> class_precision(confusion_matrix.size(),0.0);
	double precision = 0.0;

	#pragma omp parallel for num_threads(threads)	
	for(int i = 0;i<confusion_matrix.size();i++){
	
		class_precision[i] = 0.0;
		double tp = 0.0,fp = 0.0;
		for(int j = 0;j<confusion_matrix[0].size();j++){
	
			if(i==j)
				tp = confusion_matrix[i][j];
			else
				fp += confusion_matrix[i][j];
		}		
	
		class_precision[i] = double(tp/(tp+fp));

	}

	

	#pragma omp parallel for reduction(+:precision) num_threads(threads)
	for(int i = 0;i<class_precision.size();i++){

		precision += class_precision[i];

	}

	precision /= class_precision.size();

	return precision;
}

double recall_score(vector<string>& predicted_image_info,vector<string>& test_image_info,int threads){

	vector<vector<int>> confusion_matrix;
	confusion_matrix = contigency_matrix(predicted_image_info,test_image_info,threads,false);
	vector<double> class_recall(confusion_matrix.size(),0.0);
	double recall = 0.0;

	#pragma omp parallel for num_threads(threads)	
	for(int i = 0;i<confusion_matrix.size();i++){
	
		class_recall[i] = 0.0;
		double tp = 0.0,fn = 0.0;
		for(int j = 0;j<confusion_matrix[0].size();j++){
	
			if(i==j)
				tp = confusion_matrix[j][i];
			else
				fn += confusion_matrix[j][i];
		}		
	
		class_recall[i] = double(tp/(tp+fn));

	}

	

	#pragma omp parallel for reduction(+:recall) num_threads(threads)
	for(int i = 0;i<class_recall.size();i++){

		recall += class_recall[i];

	}

	recall /= class_recall.size();

	return recall;
}

double f1_score(vector<string>& predicted_image_info,vector<string>& test_image_info,int threads){

	double recall = recall_score(predicted_image_info,test_image_info,threads);
	double precision = precision_score(predicted_image_info,test_image_info,threads);

	double f1 = 2*(recall * precision)/(recall+precision);

	return f1;
  
}


#endif