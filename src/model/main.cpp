#include "header_files.h"

using namespace std;

int THREADS = 4;
int width = 92;
int height = 112;

string code_dist_m(int dist_criteria){

    if (dist_criteria == euc)
        return "euclidean";
    else if (dist_criteria == man)
        return "manhattan";
    else if (dist_criteria == che)
        return "chebyshev";
    else if (dist_criteria == hel)
        return "hellinger";
}

int main(int argv, char **argc){

    cout << argv << endl;
    string s1(argc[1]); // train dataset
    string s2(argc[2]); // test datasret
    string s3(argc[3]); //width
    string s4(argc[4]); //height
    string s5(argc[5]); //distance critereia name
    string s6(argc[6]); // k kmeans
    string s7(argc[7]); // k knn
    string s8(argc[8]); // chosse algo
    
    string filename_train, filename_test;
    
    int k_kmeans, k_knn, choose_algo, distance_measure;
    
    cout << s1 << " " << s2 << " " << s3 << " " << s4 << " " << s5 << " " << s6 << " " << s7 << " "<< s8<<endl;

    if ((int)argv != 9){

        cout << "not enough parameters .... ex ./a.out ../../data/Faces95/ReducedTrainingDataFaces95.csv ../../data/Faces95/ReducedTestFaces95.csv width height DISTANCE_MEASURE(11223344) K-KMEANS k-knn choosealgo(0123)" << endl;
        //KNN
        //./a.out ../../data/Faces95/ReducedTrainingDataFaces95.csv ../../data/Faces95/ReducedTestFaces95.csv 196 196 11 0 2 1
        //AVG_FACE
        //./a.out ../../data/Faces95/ReducedTrainingDataFaces95.csv ../../data/Faces95/ReducedTestFaces95.csv 196 196 11 0 0 2
    }
    else{

        filename_train = s1;
        filename_test = s2;
        width = atoi(s3.c_str());
        height = atoi(s4.c_str());
        distance_measure = atoi(s5.c_str());
        k_kmeans = atoi(s6.c_str());
        k_knn = atoi(s7.c_str());
        choose_algo = atoi(s8.c_str());
    }

    create_output_directories();
    
    vector<string> train_image_info;
    vector<vector<int>> train_images;
    vector<vector<int>> test_images;
    vector<string> test_image_info;
    vector<string> predicted_image_info;

    read_from_csv(train_images, train_image_info, filename_train, THREADS, 1);
    read_from_csv(test_images, test_image_info, filename_test, THREADS, 0);
    predicted_image_info.resize(test_images.size());
    double accuracy = 0.0;
    double precision = 0.0;
    double recall = 0.0;
    double f1 = 0.0;

    if (choose_algo == 0)
    { //KMEANS IS CHOSEN
        int k = k_kmeans;
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
        string sNew = str2.str();
        ofstream myfile;
        string to_open = "kmeans_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure) + ".csv";
        myfile.open(to_open.c_str());
        // myfile.open("knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure).c_str()+".csv");
        myfile << "k,dataset_name,Threads,Time,Distance_measure,Accuracy,Recall,Precision,F1score"
               << "\n";

        to_open = "Compare_kmeans_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure) + ".csv";

        ofstream tempFile;
        tempFile.open(to_open.c_str());
        // myfile.open("knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure).c_str()+".csv");
        tempFile << "Col1,Col2,Actual,Predicted"
                 << "\n";

        for (int i = 0; i < NUM_TIME; i++)
        {

            THREADS = threads[i];

            double t = omp_get_wtime();
            // cout<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n";
            // cout<<THREADS<<"\n";
            kmeans(train_images, train_image_info, k_kmeans, width, height, distance_measure, THREADS, predicted_image_info, test_images, test_image_info);
            // cout<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n";
            double f = omp_get_wtime();

            printf("time = %f\n", f - t);
            time[i] = f - t;
            if (THREADS == 1)
                for (int kk = 0; kk < predicted_image_info.size(); kk++)
                {
                    tempFile << test_images[kk][0] << "," << test_images[kk][1] << ",";
                    tempFile << test_image_info[kk] << "," << predicted_image_info[kk] << "\n";
                }

            if (predicted_image_info[0].size() > 0)
            {
                // predicted_image_info = test_image_info;
                accuracy = accuracy_score(predicted_image_info, test_image_info, THREADS);
                accuracy_list[i] = accuracy;

                recall = recall_score(predicted_image_info, test_image_info, THREADS);
                recall_list[i] = recall;

                precision = precision_score(predicted_image_info, test_image_info, THREADS);
                precision_list[i] = precision;

                f1 = f1_score(predicted_image_info, test_image_info, THREADS);
                f1_score_list[i] = f1;

                cout << "KMEANS Accuracy: " << accuracy << "\n";
                cout << "KMEANS Recall: " << recall << "\n";
                cout << "KMEANS Precision: " << precision << "\n";
                cout << "KMEANS F1 Score: " << f1 << "\n";
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
            }
            // contigency_matrix(predicted_image_info, test_image_info, THREADS, true);

            visulalize_output(test_images, predicted_image_info, test_image_info, "prediction_kmean/", width, height, THREADS);
        }
        myfile.close();
        tempFile.close();
    }
    else if (choose_algo == 1)
    { //KNN IS CHOSEN

        //KNN Predction Model

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
        str2 << k_knn;
        string sNew = str2.str();
        ofstream myfile;
        string to_open = "knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure) + ".csv";
        myfile.open(to_open.c_str());
        // myfile.open("knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure).c_str()+".csv");
        myfile << "k,dataset_name,Threads,Time,Distance_measure,Accuracy,Recall,Precision,F1score"
               << "\n";

        to_open = "Compare_knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure) + ".csv";

        ofstream tempFile;
        tempFile.open(to_open.c_str());
        // myfile.open("knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure).c_str()+".csv");
        tempFile << "Col1,Col2,Actual,Predicted"
                 << "\n";
        for (int i = 0; i < NUM_TIME; i++)
        {

            THREADS = threads[i];

            double t = omp_get_wtime();
            knn_prediction(train_images, test_images, train_image_info, predicted_image_info, k, distance_measure, THREADS);
            double f = omp_get_wtime();

            time[i] = f - t;

            printf("%f\n", f - t);
            for (int kk = 0; kk < predicted_image_info.size(); kk++)
            {
                tempFile << test_images[kk][0] << "," << test_images[kk][1] << ",";
                tempFile << test_image_info[kk] << "," << predicted_image_info[kk] << "\n";
            }

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

            visulalize_output(test_images, predicted_image_info, test_image_info, "prediction_knn/", width, height, THREADS);
        }
        myfile.close();
        tempFile.close();
    }
    else if (choose_algo == 2)
    { //MEAN(AVG) IS CHOSEN

        vector<vector<int>> avg_face, avg_face_parallel;
        vector<string> avg_face_info;
        //Means Prediction Model

        int k = 1;
        int NUM_TIME = 11;
        double time[11];

        double accuracy_list[11];
        double precision_list[11];
        double recall_list[11];
        double f1_score_list[11];

        int threads[11] = {1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};

        string temp_Name = "Original";
        std::size_t found = filename_train.find("Reduced");
        if (found != std::string::npos)
            temp_Name = "Reduced";

        ostringstream str2;
        str2 << k_knn;
        string sNew = str2.str();
        ofstream myfile;
        string to_open = "avg_face_" + temp_Name + "_" + code_dist_m(distance_measure) + ".csv";
        myfile.open(to_open.c_str());
        // myfile.open("knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure).c_str()+".csv");
        myfile << "dataset_name,Threads,Time,Distance_measure,Accuracy,Recall,Precision,F1score"
               << "\n";

        to_open = "Compare_avg_face_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure) + ".csv";

        ofstream tempFile;
        tempFile.open(to_open.c_str());
        // myfile.open("knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure).c_str()+".csv");
        tempFile << "Col1,Col2,Actual,Predicted"
                 << "\n";
        for (int i = 0; i < NUM_TIME; i++)
        {

            THREADS = threads[i];

            double t = omp_get_wtime();

            avg_face = parallel_average_face(train_images, train_image_info, avg_face_info, THREADS, width, height);
            // avg_face = generate_all_avg_face(train_images,train_image_info,avg_face_info);
            means_prediction(avg_face, test_images, avg_face_info, predicted_image_info, THREADS, distance_measure);

            double f = omp_get_wtime();
            printf("%f\n", f - t);
            time[i] = f - t;
            for (int kk = 0; kk < predicted_image_info.size(); kk++)
            {
                tempFile << test_images[kk][0] << "," << test_images[kk][1] << ",";
                tempFile << test_image_info[kk] << "," << predicted_image_info[kk] << "\n";
            }
            accuracy = accuracy_score(predicted_image_info, test_image_info, THREADS);
            accuracy_list[i] = accuracy;

            recall = recall_score(predicted_image_info, test_image_info, THREADS);
            recall_list[i] = recall;

            precision = precision_score(predicted_image_info, test_image_info, THREADS);
            precision_list[i] = precision;

            f1 = f1_score(predicted_image_info, test_image_info, THREADS);
            f1_score_list[i] = f1;

            cout << "Mean face Accuracy: " << accuracy << "\n";
            cout << "Mean face Recall: " << recall << "\n";
            cout << "Mean face Precision: " << precision << "\n";
            cout << "Mean face F1 Score: " << f1 << "\n";
            // cout << "Confusion Matrix: \n";
            ostringstream str2;
            str2 << time[i];
            string sNew = str2.str();
            myfile << filename_train << ",";
            myfile << THREADS << ",";
            myfile << sNew << ",";
            myfile << code_dist_m(distance_measure).c_str() << ",";
            myfile << accuracy << ",";
            myfile << recall << ",";
            myfile << precision << ",";
            myfile << f1 << "\n";
            // contigency_matrix(predicted_image_info, test_image_info, THREADS, true);

            visulalize_output(test_images, predicted_image_info, test_image_info, "prediction_mean/", width, height, THREADS);
        }
        myfile.close();
        tempFile.close();
    }
    else if (choose_algo == 3)
    { //ENSEMBLE IS CHOSEN

        vector<vector<int>> avg_face, avg_face_parallel;
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
        str2 << k_knn;
        string sNew = str2.str();

        str2 << k_kmeans;
        string sNewMEANS = str2.str();

        ostringstream str1;
        str1 << k_knn;
        string sNewKNN = str1.str();

        ofstream myfile;
        string to_open = "ensemble_knn_" + sNewKNN + "_KMeans_" + sNewMEANS + "_" + temp_Name + "_" + code_dist_m(distance_measure) + ".csv";
        myfile.open(to_open.c_str());

        // myfile.open("knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure).c_str()+".csv");
        myfile << "KNN,KMeans,dataset_name,Threads,Time,Distance_measure,Accuracy,Recall,Precision,F1score"
               << "\n";

        to_open = "Compare_ensemble_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure) + ".csv";

        ofstream tempFile;
        tempFile.open(to_open.c_str());
        // myfile.open("knn_" + sNew + "_" + temp_Name + "_" + code_dist_m(distance_measure).c_str()+".csv");
        tempFile << "Col1,Col2,Actual,Predicted"
                 << "\n";
        for (int i = 0; i < NUM_TIME; i++)
        {

            THREADS = threads[i];

            vector<string> predicted_KMeans;
            vector<string> predicted_KNN;
            vector<string> average_face_prediction;
            cout << "calln funcs" << endl;
            predicted_KMeans.resize(test_images.size());
            predicted_KNN.resize(test_images.size());
            average_face_prediction.resize(test_images.size());

        #pragma omp parallel 
        {
        #pragma omp sections 
        {
        #pragma omp section 
        {
                    knn_prediction(train_images, test_images, train_image_info, predicted_KNN, k_knn, distance_measure, THREADS);
                }
        #pragma omp section 
        {
                avg_face = parallel_average_face(train_images, train_image_info, avg_face_info, THREADS, width, height);
                // avg_face = generate_all_avg_face(train_images,train_image_info,avg_face_info);
                means_prediction(avg_face, test_images, avg_face_info, average_face_prediction, THREADS, distance_measure);
            }
        #pragma omp section 
        {
            kmeans(train_images, train_image_info, k_kmeans, width, height, distance_measure, THREADS, predicted_KMeans, test_images, test_image_info);
        }
        }
        }
int _size = predicted_KMeans.size();
predicted_image_info = ApplyEnsemble(predicted_KMeans, predicted_KNN, average_face_prediction, _size);

for (int kk = 0; kk < predicted_image_info.size(); kk++)
{
    tempFile << test_images[kk][0] << "," << test_images[kk][1] << ",";
    tempFile << test_image_info[kk] << "," << predicted_image_info[kk] << "\n";
}

accuracy = accuracy_score(predicted_image_info, test_image_info, THREADS);
accuracy_list[i] = accuracy;

recall = recall_score(predicted_image_info, test_image_info, THREADS);
recall_list[i] = recall;

precision = precision_score(predicted_image_info, test_image_info, THREADS);
precision_list[i] = precision;

f1 = f1_score(predicted_image_info, test_image_info, THREADS);
f1_score_list[i] = f1;

cout << " Accuracy: " << accuracy << "\n";
cout << " Recall: " << recall << "\n";
cout << " Precision: " << precision << "\n";
cout << " F1 Score: " << f1 << "\n";
//cout << "Confusion Matrix: \n";

ostringstream str2;
str2 << time[i];
string sNew = str2.str();

myfile << sNewKNN << ",";
myfile << sNewMEANS << ",";
myfile << filename_train << ",";
myfile << THREADS << ",";
myfile << sNew << ",";
myfile << code_dist_m(distance_measure).c_str() << ",";
myfile << accuracy << ",";
myfile << recall << ",";
myfile << precision << ",";
myfile << f1 << "\n";

// contigency_matrix(predicted_image_info, test_image_info, THREADS, true);

visulalize_output(test_images, predicted_image_info, test_image_info, "prediction_ensemble/", width, height, THREADS);
}
myfile.close();
tempFile.close();
}
}
