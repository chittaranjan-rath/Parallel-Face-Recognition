import os 
from PIL import Image
import numpy as np
import csv

def find_file_types():

    for folder_name in os.listdir("./faces94"):
        count = 0
        for filename in os.listdir("./faces94/" + folder_name):
            count += 1
            if '.jpg' not in filename:
                print(folder_name)

        if count != 20:
            print(folder_name)

def convert_img_to_grayscale(): 
    
    for folder_name in os.listdir("./faces94"):
        for filename in os.listdir("./faces94/" + folder_name): 

            fname = "faces94/" + folder_name + '/' + filename
            print(fname)
            img = Image.open(fname).convert('L')
            fname = "faces94/" + folder_name + '/' + "_" + filename

            img.save(fname)
                
def read_image():

    train_dataset = []
    test_dataset = []
    person_counter = 0

    for folder_name in os.listdir("./faces94"):
        
        k = 0
        person_counter += 1

        for filename in os.listdir("./faces94/" + folder_name): 

            fname = "faces94/" + folder_name + '/' + filename

            if '_' in fname:

                k += 1

                jpgfile = Image.open(fname)
                pixels = np.asarray(jpgfile, dtype=np.object)

                if k < 15:
                    train_row = []
                    train_row = ["S"+str(person_counter),"I"+str(k)]
                    train_row = train_row + list(pixels.flatten())
                    train_dataset.append(train_row)
                else:
                    test_row = []
                    test_row = ["S"+str(person_counter),"I"+str(k)]
                    test_row = test_row + list(pixels.flatten())
                    test_dataset.append(test_row)

                

    return train_dataset,test_dataset


if __name__ == '__main__': 
      
    # find_file_types()

    convert_img_to_grayscale()
    
    f = open("faces94_train_dataset.csv","w")
    f.close()

    f = open("faces94_test_dataset.csv","w")
    f.close()

    train_dataset, test_dataset = read_image()


    with open("faces94_train_dataset.csv", "a") as f:
        writer = csv.writer(f)
        writer.writerows(train_dataset)
        f.close()
    
    with open("faces94_test_dataset.csv", "a") as f:
        writer = csv.writer(f)
        writer.writerows(test_dataset)
        f.close()
    