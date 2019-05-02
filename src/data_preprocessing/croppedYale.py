import pandas as pd
import os 
from PIL import Image
import numpy as np
import csv

def read_pgm(filepath,subject_name,image_number):
    
    pgmf = open(filepath, 'rb')
    print(filepath)
    line_one = pgmf.readline()

    # print(line_one)

    # if "Ambient" in filepath:
    #     print(pgmf.readline())

    (width, height) = [int(i) for i in pgmf.readline().split()]
    # print(width,height)
    depth = int(pgmf.readline())
    # print(depth)
    # print(pgmf.readline())
    assert depth <= 255

    # print(width, height)

    subject_name = "S"+subject_name
    image_number = "I"+image_number

    raster = [subject_name,image_number]

    for y in range(height):
        for y in range(width):
            raster.append(ord(pgmf.read(1)))
    # print(len(raster))
    return raster


def read_image():

    train_dataset = []
    test_dataset = []
    person_counter = 0

    for folder_name in os.listdir("./CroppedYale"):
        
        person_counter += 1
        image_counter = 0

        for filename in os.listdir("./CroppedYale/" + folder_name): 

            if ".pgm" in filename and 'Ambient' not in filename:
                
                image_counter += 1

                fname = "CroppedYale/" + folder_name + '/' + filename

                if image_counter < 41:
                    train_dataset.append(read_pgm(fname,str(person_counter),str(image_counter)))
                else:
                    test_dataset.append(read_pgm(fname,str(person_counter),str(image_counter)))
                

    return train_dataset,test_dataset


if __name__ == '__main__': 
      
     
    train_dataset,test_dataset = read_image()
    
    f = open("cYale_train_dataset.csv","w")
    f.close()

    f = open("cYale_test_dataset.csv","w")
    f.close()

    train_dataset, test_dataset = read_image()


    with open("cYale_train_dataset.csv", "a") as f:
        writer = csv.writer(f)
        writer.writerows(train_dataset)
        f.close()
    
    with open("cYale_test_dataset.csv", "a") as f:
        writer = csv.writer(f)
        writer.writerows(test_dataset)
        f.close()
    