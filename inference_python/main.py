from preprocess import load_image, stretch_image, image64to16
from weights import load_weights_from_csv
from network import feedforward
from korean_word import convert_VAR2IDX, convert_IDX2Char

def main():
    # 사용자 입력
    image_filepath = input("Enter Image File Path: ")
    weight_filepath = input("Enter Weights File Path: ")

    # 1. 이미지 전처리
    image_array = load_image(image_filepath)
    stretched_image = stretch_image(image_array, 64)
    input_layer = image64to16(stretched_image)

    # 2. 가중치 불러오기
    num_layers, nodes_per_layer, weights = load_weights_from_csv(weight_filepath)

    # 3. 신경망 계산
    output = feedforward(input_layer, weights, nodes_per_layer)

    # 4. 결과 변환
    word_idx = convert_VAR2IDX(output)
    korean_char = convert_IDX2Char(word_idx)

    # 5. 결과 출력
    print(f"Output Unicode Character: {korean_char}")

if __name__ == "__main__":
    main()