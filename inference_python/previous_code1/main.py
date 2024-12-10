import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

import preprocess
import image_show

for i in range(1,3389):
    input_layer = preprocess.input_preprocessing(i)

    image_show.show_image_with_delay(input_layer,0.1)

    