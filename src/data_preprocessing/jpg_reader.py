import numpy as np
from PIL import Image
import csv

test_dataset = []
train_dataset = []
counter = 1
person_counter = 1

f = open("large_train_dataset.csv","w")
f.close()

f = open("large_test_dataset.csv","w")
f.close()

for i in range(1,5):
	for j in range(1,51):
		for k in range(1,15):
			if k < 10:
				filename = "originalimages_part{}/{}-0{}.jpg".format(i,person_counter,k)
			else:
				filename = "originalimages_part{}/{}-{}.jpg".format(i,person_counter,k)

			jpgfile = Image.open(filename)
			pixels = np.asarray(jpgfile, dtype=np.object)

			if k < 8:
				train_row = []
				train_row = ["S"+str(person_counter),"I"+str(k)]
				train_row = train_row + list(pixels[:,:,1].flatten())
				train_dataset.append(train_row)
			else:
				test_row = []
				test_row = ["S"+str(person_counter),"I"+str(k)]
				test_row = test_row + list(pixels[:,:,1].flatten())
				test_dataset.append(test_row)

			# dataset.append(list(pixels.flatten()))
				
			if counter == 400:
				print("write to file: ",counter)	
				with open("large_train_dataset.csv", "a") as f:
				    writer = csv.writer(f)
				    writer.writerows(train_dataset)
				    f.close()
				train_dataset = []

					
				with open("large_test_dataset.csv", "a") as f:
				    writer = csv.writer(f)
				    writer.writerows(test_dataset)
				    f.close()
				test_dataset = []


				counter = 0

			counter += 1
		
		print(person_counter)
		person_counter += 1

with open("large_train_dataset.csv", "a") as f:
    writer = csv.writer(f)
    writer.writerows(train_dataset)
    f.close()
train_dataset = []

	
with open("large_test_dataset.csv", "a") as f:
    writer = csv.writer(f)
    writer.writerows(test_dataset)
    f.close()
test_dataset = []



# jpgfile = Image.open("11-12.jpg")
# print(jpgfile.load())
# print(jpgfile.bits, jpgfile.size, jpgfile.format)
# pixels = np.asarray(jpgfile)
# pixels = list(jpgfile.getdata())
# width, height = jpgfile.size
# pixels = [pixels[i * width:(i + 1) * width] for i in range(height)]
# print(pixels.shape)
# print(type(pixels))
# print(pixels.flatten().shape)