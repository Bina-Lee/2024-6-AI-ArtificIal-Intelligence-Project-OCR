import numpy as np
from PIL import Image

def load_image(filepath):
    img = Image.open(filepath).convert("1")
    image_array = np.array(img,dtype=np.uint8)
    image_array = (image_array == 0).astype(int)

    return image_array

def stretch_image(image_array,image_size):
    r = np.zeros(64)
    c = np.zeros(64)
    for i in range(64):
        for j in range(64):
            r[i] += image_array[i,j]
            c[j] += image_array[i,j]

    r_s = 0
    c_s = 0
    r_e = 0
    c_e = 0

    for i in range(64):
        if r[i]:
            r_s = i
            break
    for i in range(64):
        if c[i]:
            c_s = i
            break
    for i in range(64):
        if r[63-i]:
            r_e = 63-i
            break
    for i in range(64):
        if c[63-i]:
            c_e = 63-i
            break

    crop_image = image_array[r_s:r_e+1, c_s:c_e+1]

    # stretched_array = np.zeros((64,64))
    stretched_array = np.zeros((image_size,image_size))

    r_size = r_e - r_s
    c_size = c_e - c_s

    for i in range(image_size):
        for j in range(image_size):
            if crop_image[round(r_size*i/(image_size-1)),round(c_size*j/(image_size-1))]:
                stretched_array[i,j] = 1
    
    return stretched_array

def image64to16(image_array):
    reduced_array = np.zeros((16,16))
    for i in range(16):
        for j in range(16):
            block = image_array[i*4:i*4+4,j*4:j*4+4]
            reduced_array[i,j] = block.mean()

    return reduced_array