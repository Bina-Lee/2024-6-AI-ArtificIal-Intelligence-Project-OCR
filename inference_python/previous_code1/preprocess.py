import numpy as np
import preprocess_function as pf

def input_preprocessing(image_num):
    image_path = image_path = f"makeImage/exportImage/text{image_num:05d}.bmp"
    image_array = pf.load_image(image_path)
    stretched_array = pf.stretch_image(image_array,64)
    image_size256 = pf.image64to16(stretched_array)

    return image_size256