#!/usr/bin/env python
# coding: utf-8

# In[2]:


import numpy as np
import pandas as pd
import math
import matplotlib.pyplot as plt
import seaborn as sns
get_ipython().magic(u'matplotlib inline')
import itertools
import random
import copy
from pprint import pprint


# In[50]:


# chebyshev_df = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Original_chebyshev.csv")
# euclidean_df = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Original_euclidean.csv")
# hellinger_df = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Original_hellinger.csv")
# manhattan_df = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Original_manhattan.csv")

# chebyshev_df = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Reduced_chebyshev.csv")
# euclidean_df = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Reduced_euclidean.csv")
# hellinger_df = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Reduced_hellinger.csv")
# manhattan_df = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Reduced_manhattan.csv")

# chebyshev_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_chebyshev.csv")
# euclidean_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_euclidean.csv")
# hellinger_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_hellinger.csv")
# manhattan_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_manhattan.csv")

# chebyshev_df_7 = pd.read_csv("./outputs/original_knn/knn_7_Original_chebyshev.csv")
# euclidean_df_7 = pd.read_csv("./outputs/original_knn/knn_7_Original_euclidean.csv")
# hellinger_df_7 = pd.read_csv("./outputs/original_knn/knn_7_Original_hellinger.csv")
# manhattan_df_7 = pd.read_csv("./outputs/original_knn/knn_7_Original_manhattan.csv")

# chebyshev_df = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_chebyshev.csv")
# euclidean_df = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_euclidean.csv")
# hellinger_df = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_hellinger.csv")
# manhattan_df = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_manhattan.csv")

# chebyshev_df = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_chebyshev.csv")
# euclidean_df = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_euclidean.csv")
# hellinger_df = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_hellinger.csv")
# manhattan_df = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_manhattan.csv")

# chebyshev_df = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_chebyshev.csv")
# euclidean_df = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_euclidean.csv")
# hellinger_df = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_hellinger.csv")
# manhattan_df = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_manhattan.csv")

chebyshev_df = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_chebyshev.csv")
euclidean_df = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_euclidean.csv")
hellinger_df = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_hellinger.csv")
manhattan_df = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_manhattan.csv")

# k = [3,7]

# threads_vs_time_vs_k_df = pd.DataFrame(
# {
# "Threads": chebyshev_df_3["Threads"],
# 'Chebyshev k = 3':chebyshev_df_3["Time"],
# 'Chebyshev k = 7':chebyshev_df_7["Time"],
# 'Euclidean k = 3': euclidean_df_3["Time"],
# 'Euclidean k = 7': euclidean_df_7["Time"],
# 'Hellinger k = 3': hellinger_df_3["Time"],
# 'Hellinger k = 7': hellinger_df_7["Time"],
# 'Manhattan k = 3': manhattan_df_3["Time"],
# 'Manhattan k = 7': manhattan_df_7["Time"]
# })
    
    
    
# #Accuracy visualisation
# threads_vs_time_vs_k_df = threads_vs_time_vs_k_df.melt('Threads', var_name='Distance Measure and K',  value_name='Time')
# threads_vs_time_vs_k_graph = sns.factorplot(x="Threads", y="Time", hue='Distance Measure and K', data=threads_vs_time_vs_k_df)
# threads_vs_time_vs_k_graph.savefig("threshold_graph.png")


# accuracy_list = []
# precision_list = []
# recall_list = []
# f1_score_list = []
# distance_measure = ["manhattan","euclidean","hellinger","chebyshev"]

# for distance in distance_measure:
    
#     if distance == "chebyshev":
#         accuracy_list.append((chebyshev_df["Accuracy"].values)[0])
#     elif distance == "manhattan":
#         accuracy_list.append((manhattan_df["Accuracy"].values)[0])
#     elif distance == "euclidean":
#         accuracy_list.append((euclidean_df["Accuracy"].values)[0])
#     elif distance == "hellinger":
#         accuracy_list.append((hellinger_df["Accuracy"].values)[0])
        
#     if distance == "chebyshev":
#         precision_list.append((chebyshev_df["Precision"].values)[0])
#     elif distance == "manhattan":
#         precision_list.append((manhattan_df["Precision"].values)[0])
#     elif distance == "euclidean":
#         precision_list.append((euclidean_df["Precision"].values)[0])
#     elif distance == "hellinger":
#         precision_list.append((hellinger_df["Precision"].values)[0])
        
#     if distance == "chebyshev":
#         recall_list.append((chebyshev_df["Recall"].values)[0])
#     elif distance == "manhattan":
#         recall_list.append((manhattan_df["Recall"].values)[0])
#     elif distance == "euclidean":
#         recall_list.append((euclidean_df["Recall"].values)[0])
#     elif distance == "hellinger":
#         recall_list.append((hellinger_df["Recall"].values)[0])
        
#     if distance == "chebyshev":
#         f1_score_list.append((chebyshev_df["F1score"].values)[0])
#     elif distance == "manhattan":
#         f1_score_list.append((manhattan_df["F1score"].values)[0])
#     elif distance == "euclidean":
#         f1_score_list.append((euclidean_df["F1score"].values)[0])
#     elif distance == "hellinger":
#         f1_score_list.append((hellinger_df["F1score"].values)[0])

        
        
# print(accuracy_list)
# print(precision_list)
# print(recall_list)
# print(f1_score_list)

# metrics_vs_distance_df = pd.DataFrame(
# {'Distance Measure': distance_measure,
#  'Accuracy':accuracy_list,
#  'Precision': precision_list,
#  'Recall': recall_list,
#  'F1 Score': f1_score_list
# })
    
# #Accuracy visualisation
# metrics_vs_distance_df = metrics_vs_distance_df.melt('Distance Measure', var_name='Metrics',  value_name='Variations')
# metrics_vs_distance_graph = sns.factorplot(x="Distance Measure", y="Variations", hue='Metrics', data=metrics_vs_distance_df)

# metrics_vs_distance_graph.savefig("threshold_graph_1.png")



threads_vs_time_df = pd.DataFrame(
    {'Threads': manhattan_df["Threads"],
     'Chebyshev':chebyshev_df["Time"],
     'Euclidean': euclidean_df["Time"],
     'Hellinger': hellinger_df["Time"],
     'Manhattan': manhattan_df["Time"]
    })
    
#Accuracy visualisation
threads_vs_time_df = threads_vs_time_df.melt('Threads', var_name='Distance Measures',  value_name='Time')
threads_vs_time_graph = sns.factorplot(x="Threads", y="Time", hue='Distance Measures', data=threads_vs_time_df)

threads_vs_time_graph.savefig("threshold_graph.png")




# lineplot_graph = sns.lineplot(x="Threads", y="Time", data=chebyshev_df)


# In[18]:


# chebyshev_df_3 = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_chebyshev.csv")
# euclidean_df_3 = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_euclidean.csv")
# hellinger_df_3 = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_hellinger.csv")
# manhattan_df_3 = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_manhattan.csv")

# chebyshev_df_7 = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_chebyshev.csv")
# euclidean_df_7 = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_euclidean.csv")
# hellinger_df_7 = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_hellinger.csv")
# manhattan_df_7 = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_manhattan.csv")

# chebyshev_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_chebyshev.csv")
# euclidean_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_euclidean.csv")
# hellinger_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_hellinger.csv")
# manhattan_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_manhattan.csv")

# chebyshev_df_7 = pd.read_csv("./outputs/original_knn/knn_7_Original_chebyshev.csv")
# euclidean_df_7 = pd.read_csv("./outputs/original_knn/knn_7_Original_euclidean.csv")
# hellinger_df_7 = pd.read_csv("./outputs/original_knn/knn_7_Original_hellinger.csv")
# manhattan_df_7 = pd.read_csv("./outputs/original_knn/knn_7_Original_manhattan.csv")


chebyshev_df_3 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_chebyshev.csv")
euclidean_df_3 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_euclidean.csv")
hellinger_df_3 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_hellinger.csv")
manhattan_df_3 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_manhattan.csv")

chebyshev_df_7 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_chebyshev.csv")
euclidean_df_7 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_euclidean.csv")
hellinger_df_7 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_hellinger.csv")
manhattan_df_7 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_manhattan.csv")

accuracy_list_3 = []
precision_list_3 = []
recall_list_3 = []
f1_score_list_3 = []

accuracy_list_7 = []
precision_list_7 = []
recall_list_7 = []
f1_score_list_7 = []

distance_measure = ["manhattan","euclidean","hellinger","chebyshev"]

for distance in distance_measure:
    
    if distance == "chebyshev":
        accuracy_list_3.append((chebyshev_df_3["Accuracy"].values)[0])
        accuracy_list_7.append((chebyshev_df_7["Accuracy"].values)[0])
    elif distance == "manhattan":
        accuracy_list_3.append((manhattan_df_3["Accuracy"].values)[0])
        accuracy_list_7.append((manhattan_df_7["Accuracy"].values)[0])
    elif distance == "euclidean":
        accuracy_list_3.append((euclidean_df_3["Accuracy"].values)[0])
        accuracy_list_7.append((euclidean_df_7["Accuracy"].values)[0])
    elif distance == "hellinger":
        accuracy_list_3.append((hellinger_df_3["Accuracy"].values)[0])
        accuracy_list_7.append((hellinger_df_7["Accuracy"].values)[0])
        
    if distance == "chebyshev":
        precision_list_3.append((chebyshev_df_3["Precision"].values)[0])
        precision_list_7.append((chebyshev_df_7["Precision"].values)[0])
    elif distance == "manhattan":
        precision_list_3.append((manhattan_df_3["Precision"].values)[0])
        precision_list_7.append((manhattan_df_7["Precision"].values)[0])
    elif distance == "euclidean":
        precision_list_3.append((euclidean_df_3["Precision"].values)[0])
        precision_list_7.append((euclidean_df_7["Precision"].values)[0])
    elif distance == "hellinger":
        precision_list_3.append((hellinger_df_3["Precision"].values)[0])
        precision_list_7.append((hellinger_df_7["Precision"].values)[0])
        
    if distance == "chebyshev":
        recall_list_3.append((chebyshev_df_3["Recall"].values)[0])
        recall_list_7.append((chebyshev_df_7["Recall"].values)[0])
    elif distance == "manhattan":
        recall_list_3.append((manhattan_df_3["Recall"].values)[0])
        recall_list_7.append((manhattan_df_7["Recall"].values)[0])
    elif distance == "euclidean":
        recall_list_3.append((euclidean_df_3["Recall"].values)[0])
        recall_list_7.append((euclidean_df_7["Recall"].values)[0])
    elif distance == "hellinger":
        recall_list_3.append((hellinger_df_3["Recall"].values)[0])
        recall_list_7.append((hellinger_df_7["Recall"].values)[0])
        
    if distance == "chebyshev":
        f1_score_list_3.append((chebyshev_df_3["F1score"].values)[0])
        f1_score_list_7.append((chebyshev_df_7["F1score"].values)[0])
    elif distance == "manhattan":
        f1_score_list_3.append((manhattan_df_3["F1score"].values)[0])
        f1_score_list_7.append((manhattan_df_7["F1score"].values)[0])
    elif distance == "euclidean":
        f1_score_list_3.append((euclidean_df_3["F1score"].values)[0])
        f1_score_list_7.append((euclidean_df_7["F1score"].values)[0])
    elif distance == "hellinger":
        f1_score_list_3.append((hellinger_df_3["F1score"].values)[0])
        f1_score_list_7.append((hellinger_df_7["F1score"].values)[0])

        
        
print(accuracy_list_3)
print(precision_list_3)
print(recall_list_3)
print(f1_score_list_3)

print(accuracy_list_7)
print(precision_list_7)
print(recall_list_7)
print(f1_score_list_7)


metrics_vs_distance_df = pd.DataFrame(
{'Distance Measure': distance_measure,
 'Accuracy Original':accuracy_list_3,
 'Accuracy Reduced':accuracy_list_7,
 'Precision Original': precision_list_3,
 'Precision Reduced': precision_list_7,
 'Recall Original': recall_list_3,
 'Recall Reduced': recall_list_7,
 'F1 Score Original': f1_score_list_3,
 'F1 Score Reduced': f1_score_list_7
})

# metrics_vs_distance_df = pd.DataFrame(
# {'Distance Measure': distance_measure,
#  'Accuracy K = 3':accuracy_list_3,
#  'Accuracy K = 7':accuracy_list_7,
#  'Precision K = 3': precision_list_3,
#  'Precision K = 7': precision_list_7,
#  'Recall K = 3': recall_list_3,
#  'Recall K = 7': recall_list_7,
#  'F1 Score K = 3': f1_score_list_3,
#  'F1 Score K = 7': f1_score_list_7
# })
    
#Accuracy visualisation
metrics_vs_distance_df = metrics_vs_distance_df.melt('Distance Measure', var_name='Metrics',  value_name='Variations')
metrics_vs_distance_graph = sns.factorplot(x="Distance Measure", y="Variations", hue='Metrics', data=metrics_vs_distance_df)

metrics_vs_distance_graph.savefig("threshold_graph_1.png")


# In[52]:


chebyshev_df_7 = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_chebyshev.csv")
euclidean_df_7 = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_euclidean.csv")
hellinger_df_7 = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_hellinger.csv")
manhattan_df_7 = pd.read_csv("./outputs/reduced_knn/knn_3_Reduced_manhattan.csv")

# chebyshev_df_7 = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_chebyshev.csv")
# euclidean_df_7 = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_euclidean.csv")
# hellinger_df_7 = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_hellinger.csv")
# manhattan_df_7 = pd.read_csv("./outputs/reduced_knn/knn_7_Reduced_manhattan.csv")

chebyshev_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_chebyshev.csv")
euclidean_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_euclidean.csv")
hellinger_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_hellinger.csv")
manhattan_df_3 = pd.read_csv("./outputs/original_knn/knn_3_Original_manhattan.csv")

# chebyshev_df_3 = pd.read_csv("./outputs/original_knn/knn_7_Original_chebyshev.csv")
# euclidean_df_3 = pd.read_csv("./outputs/original_knn/knn_7_Original_euclidean.csv")
# hellinger_df_3 = pd.read_csv("./outputs/original_knn/knn_7_Original_hellinger.csv")
# manhattan_df_3 = pd.read_csv("./outputs/original_knn/knn_7_Original_manhattan.csv")


# chebyshev_df_3 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_chebyshev.csv")
# euclidean_df_3 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_euclidean.csv")
# hellinger_df_3 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_hellinger.csv")
# manhattan_df_3 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Original_manhattan.csv")

# chebyshev_df_7 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_chebyshev.csv")
# euclidean_df_7 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_euclidean.csv")
# hellinger_df_7 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_hellinger.csv")
# manhattan_df_7 = pd.read_csv("./outputs/kmeans_red_ori/kmeans_72_Reduced_manhattan.csv")


# chebyshev_df_3 = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Original_chebyshev.csv")
# euclidean_df_3 = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Original_euclidean.csv")
# hellinger_df_3 = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Original_hellinger.csv")
# manhattan_df_3 = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Original_manhattan.csv")

# chebyshev_df_7 = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Reduced_chebyshev.csv")
# euclidean_df_7 = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Reduced_euclidean.csv")
# hellinger_df_7 = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Reduced_hellinger.csv")
# manhattan_df_7 = pd.read_csv("./outputs/avg_face_red_ori/avg_face_Reduced_manhattan.csv")


threads_vs_time_vs_k_df = pd.DataFrame(
{
"Threads": chebyshev_df_3["Threads"],
'Chebyshev Original':chebyshev_df_3["Time"],
'Chebyshev Reduced':chebyshev_df_7["Time"],
'Euclidean Original': euclidean_df_3["Time"],
'Euclidean Reduced': euclidean_df_7["Time"],
'Hellinger Original': hellinger_df_3["Time"],
'Hellinger Reduced': hellinger_df_7["Time"],
'Manhattan Original': manhattan_df_3["Time"],
'Manhattan Reduced': manhattan_df_7["Time"]
})

#Accuracy visualisation
threads_vs_time_vs_k_df = threads_vs_time_vs_k_df.melt('Threads', var_name='Distance Measure and Dataset Type',  value_name='Time')
threads_vs_time_vs_k_graph = sns.factorplot(x="Threads", y="Time", hue='Distance Measure and Dataset Type', data=threads_vs_time_vs_k_df)
threads_vs_time_vs_k_graph.savefig("threshold_graph.png")


# In[62]:


# df = pd.read_csv("./outputs/kmeans_72_Original.csv")
# df = pd.read_csv("./outputs/Compare_knn_3_Reduced_euclidean.csv")
# df = pd.read_csv("./outputs/new_datasets_op/yale_avg_face/Compare_avg_face_0_Reduced_euclidean.csv")
# df = pd.read_csv("./outputs/new_datasets_op/yale_knn_3/Compare_knn_3_Reduced_euclidean.csv")

df = pd.read_csv("./outputs/new_datasets_op/ensemble_red/Compare_ensemble_3_Reduced_euclidean.csv")



X = df.values[:, :2] 
y_actual = df.values[:, 2]
y_predicted = df.values[:, 3]

# print(X)
# print(y_actual)
# print(y_predicted)

plt.title('Prediction on reduced testing data')
plt.xlabel('Principal Component 1')
plt.ylabel('Principal Component 2')
# plt.scatter(X[:, 0], X[:, 1], c=y_actual, s=40, cmap='viridis')
plt.scatter(X[:, 0], X[:, 1], c=y_predicted, s=40, cmap='viridis')
plt.savefig("test.png")


# In[ ]:





# In[ ]:




