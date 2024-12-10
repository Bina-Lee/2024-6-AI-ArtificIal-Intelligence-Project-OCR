import matplotlib.pyplot as plt

def show_single_image(input_layer):
    # 256개의 입력값을 16x16 배열로 변환
    # input_image = input_layer.reshape((16, 16))
    
    # plt.imshow(input_image, cmap='gray', vmin=0, vmax=1)
    plt.imshow(input_layer, cmap='gray', vmin=0, vmax=1)
    plt.colorbar()  # 색상 바 추가하여 값을 더 직관적으로 확인 가능
    plt.title("Input Layer Visualized as Image")
    plt.show()

def show_image_with_delay(input_layer,pause_time):
    plt.clf()  # 이전 이미지를 지워 새로운 이미지를 그릴 준비를 합니다.
    plt.imshow(input_layer, cmap='gray', vmin=0, vmax=1)
    plt.colorbar()  # 색상 바 추가하여 값을 더 직관적으로 확인 가능
    plt.title("Input Layer Visualized as Image")
    plt.draw()  # 이미지를 그립니다.
    plt.pause(pause_time)  # 0.5초 동안 대기하여 갱신이 보이도록 합니다.