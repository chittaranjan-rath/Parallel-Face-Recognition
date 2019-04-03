import pandas as pd

def read_pgm(filepath,subject_name,image_number):
    
    pgmf = open(filepath, 'rb')

    line_one = pgmf.readline()
    (width, height) = [int(i) for i in pgmf.readline().split()]
    depth = int(pgmf.readline())
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

train_num_list = []
test_num_list = []

for i in range(1,41):
    for j in range(1,11):
        filepath = "/home/prakashjha/semester2/IPSC/project/att_faces/orl_faces/s{}/{}.pgm".format(str(i),str(j))

        if j < 9:
            train_num_list.append(read_pgm(filepath,str(i),str(j)))
        else:
            test_num_list.append(read_pgm(filepath,str(i),str(j)))
        

train_image_df = pd.DataFrame(train_num_list)
test_image_df = pd.DataFrame(test_num_list)

train_image_df.to_csv('train_image_dataset.csv', index=False, header=None)
test_image_df.to_csv('test_image_dataset.csv', index=False, header=None)

print(train_image_df.shape)
print(test_image_df.shape)