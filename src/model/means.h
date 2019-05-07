#ifndef MEANS_H
#define MEANS_H

vector<int> average_face(vector<vector<int> > &image_pixel, vector<string> &img_info, string subject_id)
{

    vector<int> avg_face;
    int count = 0;

    for (int i = 0; i < image_pixel.size(); i++)
    {

        if (img_info[i] == subject_id)
        {

            if (avg_face.size() == 0)
            {
                count++;
                avg_face = image_pixel[i];
            }
            else
            {
                count++;
                for (int j = 0; j < image_pixel[i].size(); j++)
                {
                    avg_face[j] += image_pixel[i][j];
                }
            }
        }
    }

    for (int i = 0; i < avg_face.size(); i++)
    {
        avg_face[i] = (int)(avg_face[i] / count);
    }

    return avg_face;
}

vector<vector<int>> parallel_average_face(vector<vector<int>> &image_pixel, vector<string> &img_info, vector<string> &avg_face_info, int THREADS, int width, int height)
{

    cout << IMAGES_PER_SUBJECT << "\n";
    int num_subjects = int(img_info.size() / IMAGES_PER_SUBJECT);

    vector<int> start_index(num_subjects);
    vector<int> end_index(num_subjects);

    vector<vector<int>> avg_face;

    avg_face.resize(num_subjects, vector<int>(image_pixel[0].size()));
    avg_face_info.resize(num_subjects);

    string str = "avg_face_parallel/";
    // #pragma omp parallel
    // {
    // #pragma omp nowait
#pragma omp parallel for num_threads(THREADS)
    for (int i = 0; i < num_subjects; i++)
    {

        avg_face_info[i] = img_info[IMAGES_PER_SUBJECT * i];

        start_index[i] = IMAGES_PER_SUBJECT * i;
        end_index[i] = IMAGES_PER_SUBJECT * (i + 1) - 1;

#pragma omp collaspe(2) nowait
        for (int j = start_index[i]; j <= end_index[i]; j++)
        {

            for (int k = 0; k < image_pixel[0].size(); k++)

                avg_face[i][k] += image_pixel[j][k];
        }
    }

    // #pragma omp parallel for num_threads(THREADS)
#pragma omp collaspe(2) nowait
    for (int i = 0; i < avg_face.size(); i++)
    {

        for (int k = 0; k < avg_face[0].size(); k++)
            avg_face[i][k] /= IMAGES_PER_SUBJECT;
            ostringstream str2;
            str2 << i;
            string sNew = str2.str();
        print_image(avg_face[i], str + sNew, width, height);
    }

    return avg_face;
}

vector<vector<int>> generate_all_avg_face(vector<vector<int>> &image_pixel, vector<string> &img_info, vector<string> &avg_face_info, int width, int height)
{

    vector<vector<int>> avg_face;
    string filename = "";
    string str = "avg_face/";
    string subject_id = "";
    int count = 0;

    for (int i = 0; i < image_pixel.size(); i++)
    {

        if (subject_id != img_info[i])
        {

            avg_face_info.push_back(img_info[i]);

            filename = img_info[i];
            // print_image(image_pixel[i],filename);

            subject_id = img_info[i];
            // parallel_average_face(image_pixel,img_info,img_info[i]);
            avg_face.push_back(average_face(image_pixel, img_info, img_info[i]));

            filename = img_info[i] + "avg";

            print_image(avg_face[count], str + filename, width, height);

            count++;
        }
    }
    return avg_face;
}

void means_prediction(vector<vector<int>> &avg_face, vector<vector<int>> &test_images, vector<string> &avg_face_info, vector<string> &predicted_image_info, int THREADS, int dist_criteria)
{

    vector<double> min_distance(test_images.size());

#pragma omp parallel for num_threads(THREADS)
    for (int i = 0; i < test_images.size(); i++)
    {
        cout << "processing test image: " << i << "\n";
        double temp;
        min_distance[i] = 0.0;

#pragma omp parallel for schedule(static) num_threads(THREADS)
        for (int j = 0; j < avg_face.size(); j++)
        {

            if (j == 0)
            {

                // min_distance[i] = euclidean_distance(avg_face[j], test_images[i], THREADS);
                // min_distance[i] = manhattan_distance(avg_face[j],test_images[i],THREADS);
                // min_distance[i] = chebyshev_distance(avg_face[j],test_images[i],THREADS);
                // min_distance[i] = hellinger_distance(avg_face[j],test_images[i],THREADS);

                if (dist_criteria == euc)
                    min_distance[i] = euclidean_distance(avg_face[j], test_images[i], THREADS);
                else if (dist_criteria == man)
                    min_distance[i] = manhattan_distance(avg_face[j], test_images[i], THREADS);
                else if (dist_criteria == che)
                    min_distance[i] = chebyshev_distance(avg_face[j], test_images[i], THREADS);
                else if (dist_criteria == hel)
                    min_distance[i] = hellinger_distance(avg_face[j], test_images[i], THREADS);

                predicted_image_info[i] = avg_face_info[j];
            }
            else
            {

                // temp = euclidean_distance(avg_face[j], test_images[i], THREADS);
                // temp = manhattan_distance(avg_face[j],test_images[i],THREADS);
                // temp = chebyshev_distance(avg_face[j],test_images[i],THREADS);
                // temp = hellinger_distance(avg_face[j],test_images[i],THREADS);

                if (dist_criteria == euc)
                    temp = euclidean_distance(avg_face[j], test_images[i], THREADS);
                else if (dist_criteria == man)
                    temp = manhattan_distance(avg_face[j], test_images[i], THREADS);
                else if (dist_criteria == che)
                    temp = chebyshev_distance(avg_face[j], test_images[i], THREADS);
                else if (dist_criteria == hel)
                    temp = hellinger_distance(avg_face[j], test_images[i], THREADS);



                if (temp < min_distance[i])
                {

                    min_distance[i] = temp;
                    predicted_image_info[i] = avg_face_info[j];
                }
            }
        }
    }
}
#endif