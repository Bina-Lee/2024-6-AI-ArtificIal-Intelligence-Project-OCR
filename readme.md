# OCR Model for Korean Characters (Final Project for AI Course)

## Overview
This repository contains the source code and resources for the final project in the **Artificial Intelligence (AI)** course during the 6th semester of 2024. The project implements an Optical Character Recognition (OCR) system for Korean characters using Artificial Neural Networks (ANN) in **C language**. The project explores how a 64x64 Korean character bitmap image is processed to predict the character using an ANN model.

## Project Features
- **Model Architecture**: ANN with:
  - 5 to 16 layers
  - Up to 256 nodes per layer
- **Input**: Downsampled 16x16 image derived from a 64x64 bitmap
- **Output**: Unicode indices for the initial, medial, and final consonants of Korean characters
- **Implementation**:
  - Training, inference, and accuracy evaluation implemented in C.
  - Auxiliary Python scripts for generating and preprocessing images.

---

## Repository Structure
```.
├── makeImage/
│   ├── exportImage/      # Exported images used as input for the model
│   ├── font/             # Empty folder (Fonts omitted for copyright reasons)
│   └── [Python Scripts]  # Image generation tools
├── inference_python/
│   └── [Python Scripts]  # Simple inference model structure for reference
├── training_c/
│   ├── training.c        # Model training implementation
│   ├── inference.c       # Character prediction using trained weights
│   ├── accuracy.c        # Accuracy calculation using all generated text images
└── README.md             # This file
```
---

## Details

### 1. Image Preprocessing
- **Python Script**: Located in the `makeImage` directory.
- **Input**: Korean characters using user-provided fonts.
- **Output**: 64x64 1-bit bitmap images of characters stored in `makeImage/exportImage`.
- **Additional Steps**: Images downsampled to 16x16 resolution for ANN input.

### 2. Model Training
- **Source**: `training_c/training.c`
- **Description**: Trains the ANN model on the 16x16 processed images.
- **Output**: Weight files saved based on layer and node configuration.

### 3. Inference
- **Source**: `training_c/inference.c`
- **Description**: Loads the trained weights and predicts the character for a given image.

### 4. Accuracy Evaluation
- **Source**: `training_c/accuracy.c`
- **Description**: Tests model accuracy using all images in `makeImage/exportImage`.

---

## Dependencies
- **C Compiler**: GCC or compatible compiler for compiling and running the C programs.
- **Python Environment**: For image generation (requires `Pillow` library).

---

## Limitations and Known Issues
- **Font Directory**: The `makeImage/font` folder is empty due to copyright concerns. Users must add their own font files to this folder to generate the dataset.  
  - The Python script is designed to read font files from the `font` directory. Adding fonts to this directory will enable the scripts to function correctly.
- **Python Inference**: A simple reference model structure is included in `inference_python`, but the actual inference process is implemented in C.
- **Accuracy Measurement**: The accuracy logic implemented in C is more reliable, while the Python implementation is only provided for reference.  
  - The `accuracy.c` file can be used to measure the accuracy of the model across the entire dataset.

---

## Usage

### 1. Generate Training Data
```bash
cd makeImage
python code.py
```
### 2. Train the Model
```bash
cd training_c
gcc training.c load_n_preprocess/image_load.c load_n_preprocess/preprocessing.c load_n_preprocess/load_data.c math_func/math_func.c create_network/create_layer_n_weight.c create_network/random_weight.c load_n_preprocess/load_target.c word_inference/word_inference.c csv_operate/export_weight_csv.c -I./load_n_preprocess -I./math_func -I./create_network -I./word_inference -I./typedef_struct -o training -lm -v
./training
```
### 3. Perform Inference
```bash
gcc inference.c load_n_preprocess/image_load.c load_n_preprocess/preprocessing.c load_n_preprocess/load_data.c math_func/math_func.c create_network/create_layer_n_weight.c create_network/random_weight.c load_n_preprocess/load_target.c word_inference/word_inference.c csv_operate/import_weight_csv.c csv_operate/export_weight_csv.c -I./load_n_preprocess -I./math_func -I./create_network -I./word_inference -I./typedef_struct -o inference -lm -v
./inference
```
### 4. Evaluate Accuracy
```bash
gcc accuracy.c load_n_preprocess/image_load.c load_n_preprocess/preprocessing.c load_n_preprocess/load_data.c math_func/math_func.c create_network/create_layer_n_weight.c create_network/random_weight.c load_n_preprocess/load_target.c word_inference/word_inference.c csv_operate/import_weight_csv.c csv_operate/export_weight_csv.c -I./load_n_preprocess -I./math_func -I./create_network -I./word_inference -I./typedef_struct -o accuracy -lm -v
./accuracy
```
### References

Refer to the accompanying PDF report for detailed explanations about the implementation, experimental setup, and results. Specifically, see Section 3 for experimental methods.


# 한글 OCR 모델 (6학기 인공지능 기말 프로젝트)

## 개요
- **2024년도 6학기 인공지능 과목** 기말 프로젝트의 소스 코드와 자료를 포함함. 
- 프로젝트는 **C**로 구현된 한글에 대한 OCR 시스템임. 
- 64x64 크기의 한글 비트맵 이미지를 입력으로 받아, ANN을 통해 해당 문자를 예측함.

## 프로젝트 주요 내용
- **모델 구조**: 
  - 5~16개의 레이어
  - 레이어당 최대 256개의 노드
- **입력 데이터**: 64x64 비트맵 이미지를 16x16으로 다운샘플링하여 사용
- **출력 데이터**: 한글의 초성, 중성, 종성의 유니코드 인덱스
- **구현 언어**: 
  - C로 학습, 추론, 정확도 평가 구현
  - Python으로 이미지 생성 및 전처리 보조 구현

---

## 디렉토리 구조
```
.
├── makeImage/
│   ├── exportImage/      # 모델 입력으로 사용될 출력 이미지
│   ├── font/             # 빈 폴더 (저작권 문제로 폰트 미포함)
│   └── [Python Scripts]  # 이미지 생성 도구
├── inference_python/
│   └── [Python Scripts]  # 참조용 간단한 추론 모델 구조
├── training_c/
│   ├── training.c        # 모델 학습 구현
│   ├── inference.c       # 학습된 가중치로 문자 예측
│   ├── accuracy.c        # 생성된 모든 텍스트 이미지로 정확도 측정
└── README.md             # 본 파일
```
---

## 세부 내용

### 1. 이미지 전처리
- **Python 스크립트**: `makeImage` 디렉토리에 위치
- **입력**: 사용자가 제공한 폰트를 이용한 한글 문자
- **출력**: `makeImage/exportImage`에 저장된 64x64 1비트 비트맵 이미지
- **추가 작업**: ANN 입력으로 사용하기 위해 이미지를 16x16으로 다운샘플링

### 2. 모델 학습
- **소스 코드**: `training_c/training.c`
- **설명**: 16x16으로 처리된 이미지를 사용하여 ANN 모델 학습
- **출력**: 레이어와 노드 구성에 따라 저장된 가중치 파일

### 3. 추론
- **소스 코드**: `training_c/inference.c`
- **설명**: 학습된 가중치를 불러와 주어진 이미지의 문자를 예측

### 4. 정확도 평가
- **소스 코드**: `training_c/accuracy.c`
- **설명**: `makeImage/exportImage`에 있는 모든 이미지로 모델의 정확도 측정

---

## 의존성
- **C 컴파일러**: GCC 또는 호환되는 컴파일러
- **Python 환경**: 이미지 생성을 위한 `Pillow` 라이브러리 필요

---

## 제한 사항 및 알려진 문제
- **폰트 폴더**: 저작권 문제로 `makeImage/font` 폴더는 비어 있으며, 데이터셋 생성을 위해 사용자가 직접 폰트 파일을 추가해야 합니다.
  - Python 폴더 내에서 font 디렉토리 내의 폰트 파일을 긁어서 동작하도록 되어있으므로 해당 폴더에 폰트를 넣어서 동작시키면 정상적으로 동작함
- **Python 추론**: `inference_python`에 간단한 구조의 참조용 모델이 포함되어 있으나, 실제 동작은 C로 구현된 추론을 통해 이루어짐
- **정확도 측정**: 정확도 측정을 위한 로직은 C 구현이 더 신뢰할 수 있으며, Python 구현은 참조용으로만 사용됩니다.
  - 별도의 accuracy.c를 통해 해당모델의 전체 데이터셋에 대한 정확도를 측정할 수 있음

---

## 사용법

### 1. 학습 데이터 생성
```bash
cd makeImage
python code.py
```
### 2. 모델 학습
```bash
cd training_c
gcc training.c load_n_preprocess/image_load.c load_n_preprocess/preprocessing.c load_n_preprocess/load_data.c math_func/math_func.c create_network/create_layer_n_weight.c create_network/random_weight.c load_n_preprocess/load_target.c word_inference/word_inference.c csv_operate/export_weight_csv.c -I./load_n_preprocess -I./math_func -I./create_network -I./word_inference -I./typedef_struct -o training -lm -v
./training
```
### 3. 모델 추론
```bash
gcc inference.c load_n_preprocess/image_load.c load_n_preprocess/preprocessing.c load_n_preprocess/load_data.c math_func/math_func.c create_network/create_layer_n_weight.c create_network/random_weight.c load_n_preprocess/load_target.c word_inference/word_inference.c csv_operate/import_weight_csv.c csv_operate/export_weight_csv.c -I./load_n_preprocess -I./math_func -I./create_network -I./word_inference -I./typedef_struct -o inference -lm -v
./inference
```
### 4. 정확도 평가
```bash
gcc accuracy.c load_n_preprocess/image_load.c load_n_preprocess/preprocessing.c load_n_preprocess/load_data.c math_func/math_func.c create_network/create_layer_n_weight.c create_network/random_weight.c load_n_preprocess/load_target.c word_inference/word_inference.c csv_operate/import_weight_csv.c csv_operate/export_weight_csv.c -I./load_n_preprocess -I./math_func -I./create_network -I./word_inference -I./typedef_struct -o accuracy -lm -v
./accuracy
```

참고 자료

구현, 실험 방법, 결과에 대한 자세한 설명은 report PDF 보고서를 참조하십시오.


function 및 header 호출 관계
```C
main.c
├──load_n_preprocess/load_date.c
│   └──load_n_preprocess/image_load.c
│       └──typedef_struct/image_struct.h
├──load_n_preprocess/preprocessing.c
│   └──load_n_preprocess/preprocessing.h
├──load_n_preprocess/load_target.c
│   └──typedef_struct/korean_word.h
├──math_func/math_func.c
├──create_network/create_layer_n_weight.c
│   └──create_network/random_weight.c
├──word_inference/word_inference.c
│   └──typedef_struct/korean_word.h

├──csv_operate/export_weight_csv.c
```
