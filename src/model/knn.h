
#ifndef KNN_H
#define KNN_H
void knn_prediction(vector<vector<int>> &train_image, vector<vector<int>> &test_image, vector<string> &train_image_info,
                    vector<string> &predicted_image_info, int k, int dist_criteria, int THREADS)
{

  double min_distance = 0.0, temp;
  int num_subjects = train_image.size() / IMAGES_PER_SUBJECT;

#pragma omp parallel for num_threads(THREADS)
  for (int i = 0; i < test_image.size(); i++)
  {
    cout << "Processing test image " << i << "\n";
    vector<pair<double, string>> distances(train_image.size());
    vector<int> vote(num_subjects, 0);

#pragma omp parallel for num_threads(THREADS)
    for (int j = 0; j < train_image.size(); j++)
    {

      if (dist_criteria == euc)
        distances[j].first = euclidean_distance(test_image[i], train_image[j], THREADS);
      else if (dist_criteria == man)
        distances[j].first = manhattan_distance(test_image[i], train_image[j], THREADS);
      else if (dist_criteria == che)
        distances[j].first = chebyshev_distance(test_image[i], train_image[j], THREADS);
      else if (dist_criteria == hel)
        distances[j].first = hellinger_distance(test_image[i], train_image[j], THREADS);

      distances[j].second = train_image_info[j];
    }

    sort(distances.begin(), distances.end());

#pragma omp parallel for num_threads(THREADS)
    for (int j = 0; j < k; j++)
    {

      string temp = distances[j].second;

      temp.erase(temp.begin());

      int index = atoi(temp.c_str()) - 1;

      vote[index] += 1;
    }

    int max_vote = INT_MIN;
    string prediction = "";

#pragma omp parallel for num_threads(THREADS)
    for (int j = 0; j < vote.size(); j++)
    {

      if (vote[j] > max_vote)
      {
        max_vote = vote[j];
        ostringstream str2;
        str2 << (j + 1);
        string sNew = str2.str();
        prediction = "S" + sNew;
      }
    }

    predicted_image_info[i] = prediction;
  }
}
#endif
