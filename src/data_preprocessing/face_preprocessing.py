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

num_list = []

for i in range(1,41):
    for j in range(1,11):
        filepath = "/home/prakashjha/semester2/IPSC/project/att_faces/orl_faces/s{}/{}.pgm".format(str(i),str(j))

        num_list.append(read_pgm(filepath,str(i),str(j)))
        # print((num_list))


image_df = pd.DataFrame(num_list)

image_df.to_csv('image_dataset.csv', index=False, header=None)

print(image_df.shape)