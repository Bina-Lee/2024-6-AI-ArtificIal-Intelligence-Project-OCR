import numpy as np
from PIL import Image
import preprocess_function

for i in range(1, 3389):
    image_path = f"makeImage/exportImage/text{i:05d}.bmp"
    output_path = f"inputLayerSample/input{i:05d}.bmp"

    # 이미지 불러오기 및 처리
    image_array = preprocess_function.load_image(image_path)
    input_layer = preprocess_function.stretch_image(image_array,64)
    input_layer = preprocess_function.image64to16(input_layer)

    # numpy 배열을 0과 255로 변환하여 이미지로 변환
    input_layer = (input_layer * 255).astype(np.uint8)
    image = Image.fromarray(input_layer)  # numpy 배열을 Image 객체로 변환
    image = image.convert("1")  # 1비트 비트맵으로 변환

    # 처리된 이미지 저장
    image.save(output_path, dpi=(16, 16))

print("모든 이미지가 처리되어 저장되었습니다.")