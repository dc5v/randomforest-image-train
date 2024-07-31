# randomforest-image-train

Tensorflow 쓰기싫어서 걍 만들어봤습니다.

랜덤포레스트에 대해 이해하기 쉽도록, 단순하고 심플하게 작성했습니다.

누군가 도움이 되기를 바라며...

## Build

```sh
mkdir build
cd build
cmake ..
make
```

## Test

`/test/test` 파일을 작성했음. 요긴하게 쓰세용

```sh
cd test
./test [...cpp] [optional...args]

# i.e. 
# ./test image.test.cpp origin.jpg
```

## TreeNode

`randomforest.hpp`
```cpp
struct TreeNode
{
    int feature;
    double threshold;
    double value;
    TreeNode *left;
    TreeNode *right;
};
```

- feature: 분할 인덱스
- threshold: 분할 임계값
- value: 예측 값
- left, right: 좌우 자식 노드


## Parameters

`paramters.hpp`
```cpp
#define MODEL_FILENAME "randomforest-image-train.model"
#define GAUSSIAN_SIZE 5
#define COLOR_LIMIT 3
#define FOREST_DEPTH 10
#define FOREST_TREES 5
```
- MODEL_FILENAME: 저장할 모델파일명
- GAUSSIAN_SIZE: 가우시안 블러 사이즈
- COLOR_LIMIT: 포스터라이즈 컬러제한수
- FOREST_DEPTH: 랜덤포레스트 노드별 최대 깊이
- FOREST_TREES: 랜덤포레스트 노드의 최대 나무 수

## 평균, 분산, MSE
  
  ```math
  평균 = \frac{1}{n} \sum_{i=1}^{n} x_i
  ```
  
  ```math
  분산 = \frac{1}{n} \sum_{i=1}^{n} (x_i - \mu)^2
  ```
  
  ```math
  MSE = \frac{N_{left} \cdot \sigma^2_{left} + N_{right} \cdot \sigma^2_{right}}{N_{left} + N_{right}}
  ```


## License

Unlicense. 

> Always unlicense 🫶🏼

