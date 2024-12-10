import numpy as np
from PIL import Image

def load_image(filepath):
    img = Image.open(filepath).convert("1")
    return np.array(img, dtype=np.uint8)

def crop_index(image_array):
    rows, cols = np.any(image_array, axis=1), np.any(image_array, axis=0)
    r_s, r_e = np.where(rows)[0][[0, -1]]
    c_s, c_e = np.where(cols)[0][[0, -1]]
    return r_s, r_e, c_s, c_e

def stretch_image(image_array, size):
    r_s, r_e, c_s, c_e = crop_index(image_array)
    cropped = image_array[r_s:r_e+1, c_s:c_e+1]
    return np.array(Image.fromarray(cropped).resize((size, size), Image.NEAREST))

def image64to16(image_array):
    reduced = np.zeros((16, 16))
    for i in range(16):
        for j in range(16):
            block = image_array[i*4:(i+1)*4, j*4:(j+1)*4]
            reduced[i, j] = block.mean()
    return reduced