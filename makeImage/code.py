from PIL import Image, ImageDraw, ImageFont
import os

# 텍스트 설정
text = ""
unique_chars = set(text)  # 중복 제거
print(len(unique_chars))

# 저장할 폴더 경로 및 텍스트 파일 경로
output_folder = "makeImage/exportImage"
os.makedirs(output_folder, exist_ok=True)  # 폴더가 없으면 생성
index_file_path = "makeImage/textIndexCode.txt"

# 폰트 폴더 경로
font_folder = "makeImage/font"
file_index = 1  # 파일 번호 초기화

# 초성, 중성, 종성 인덱스 계산 함수
def get_jamo_indices(char):
    code = ord(char) - 0xAC00
    cho = code // (21 * 28)  # 초성 인덱스
    jung = (code % (21 * 28)) // 28  # 중성 인덱스
    jong = code % 28  # 종성 인덱스
    return cho, jung, jong

# 텍스트 파일 열기
with open(index_file_path, "w") as index_file:
    # 각 폰트를 순회하며 동일한 텍스트를 이미지로 생성
    for font_file in os.listdir(font_folder):
        font_path = os.path.join(font_folder, font_file)

        # 폰트 파일이 .ttf나 .ttc인지 확인 (대소문자 무관)
        if font_file.lower().endswith((".ttf", ".ttc")):
            try:
                font = ImageFont.truetype(font_path, 40)  # 글자 크기 조정 가능

                # 각 중복 제거된 문자를 개별 이미지로 생성 및 저장
                for char in unique_chars:
                    # 64x64 크기의 1비트 흑백 이미지 생성 (흰색 배경)
                    image = Image.new('1', (64, 64), color=0)
                    draw = ImageDraw.Draw(image)
                    
                    # 텍스트 위치 계산 (중앙 정렬)
                    bbox = draw.textbbox((0, 0), char, font=font)
                    text_width, text_height = bbox[2] - bbox[0], bbox[3] - bbox[1]
                    text_x = (64 - text_width) // 2
                    text_y = (64 - text_height) // 2

                    # 텍스트 그리기 (검정색)
                    draw.text((text_x, text_y), char, font=font, fill=1)
                    
                    # 텍스트가 렌더링되지 않은 경우 건너뛰기
                    if all(pixel == 0 for pixel in image.getdata()):
                        print(f"Skipping {char} for font {font_file} (text not rendered)")
                        continue

                    # 파일 이름 설정 및 저장
                    filename = f"text{file_index:05d}.bmp"
                    file_path = os.path.join(output_folder, filename)
                    image.save(file_path, dpi=(64, 64))
                    print(f"Saved {file_path}")

                    # 초성, 중성, 종성 인덱스 계산
                    cho, jung, jong = get_jamo_indices(char)
                    index_file.write(f"text{file_index:05d}_{cho:02d}_{jung:02d}_{jong:02d}\n")
                    # 0~18 0~20 0~27

                    file_index += 1  # 파일 인덱스 증가
                    
            except Exception as e:
                print(f"Error loading font {font_file}: {e}")

# font : 28 / char : 111