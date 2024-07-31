# randomforest-image-train

Tensorflow 쓰기싫어서 걍 만들어봄;

## Build

```sh
mkdir build
cd build
cmake ..
make
```

## TreeNode

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
