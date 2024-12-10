How to compile
- training.c
```bash
gcc training.c load_n_preprocess/image_load.c load_n_preprocess/preprocessing.c load_n_preprocess/load_data.c math_func/math_func.c create_network/create_layer_n_weight.c create_network/random_weight.c load_n_preprocess/load_target.c word_inference/word_inference.c csv_operate/export_weight_csv.c -I./load_n_preprocess -I./math_func -I./create_network -I./word_inference -I./typedef_struct -o training -lm -v
```

- inference.c
```bash
gcc inference.c load_n_preprocess/image_load.c load_n_preprocess/preprocessing.c load_n_preprocess/load_data.c math_func/math_func.c create_network/create_layer_n_weight.c create_network/random_weight.c load_n_preprocess/load_target.c word_inference/word_inference.c csv_operate/import_weight_csv.c csv_operate/export_weight_csv.c -I./load_n_preprocess -I./math_func -I./create_network -I./word_inference -I./typedef_struct -o inference -lm -v
```

- accuracy.c
```bash
gcc accuracy.c load_n_preprocess/image_load.c load_n_preprocess/preprocessing.c load_n_preprocess/load_data.c math_func/math_func.c create_network/create_layer_n_weight.c create_network/random_weight.c load_n_preprocess/load_target.c word_inference/word_inference.c csv_operate/import_weight_csv.c csv_operate/export_weight_csv.c -I./load_n_preprocess -I./math_func -I./create_network -I./word_inference -I./typedef_struct -o accuracy -lm -v
```


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


makeImage/font 내의 폰트파일을 별도로 git에 작성하지 않음
저작권문제가 있기 떄문에....
