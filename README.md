# randomforest-image-train



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

- 평균 
  
  $$
  \{ \frac{1}{n} \sum_{i=1}^{n} x_i \}
  $$

- 분산
  
  $$
  \{ \frac{1}{n} \sum_{i=1}^{n} (x_i - \mu)^2 \}
  $$
  
- MSE

  \[ MSE = \frac{N_{left} \cdot \sigma^2_{left} + N_{right} \cdot \sigma^2_{right}}{N_{left} + N_{right}} \]
