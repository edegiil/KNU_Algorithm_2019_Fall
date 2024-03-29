# COMP319 Algorithms
### Visiting maximum points with limited fuel problem
2015112182 박기덕

#### 1. 아이디어
 기본적인 방향이 필요하다고 생각했다. 왜냐하면 다른 방향으로 가면 (100, 100)으로 돌아오기 어려울 수 있기 때문이다. 딥러닝의 아주 기본적인 배경인 **Linear Regression**의 컨셉을 차용하기로 했다. Linear Regression은 2차원 상의 여러개의 점 들의 x와 y간의 상관관계를 모델링하는 회귀분석 기법인데, 이를 통해 한정된 자원으로 두 점 (0, 0)과 (100, 100) 사이의 점들을 방문하려면 두 점을 이은 직선과 가까운 점들을 방문해야 한다는 아이디어를 얻을 수 있었다. 저 직선에서 과도하게 벗어날 경우 자원이 많이 소비되기에 어느정도 범위 내에 있는 점들을 방문하면 점들 간에 이동하는 거리를 최소화 하여 최대한 많은 점들을 방문 할 수 있을 것이라 생각했다.


#### 2. 구현
 가장 먼저 무작위의 점들을 이용해서 점들을 2차원 평면상에 나타내는 map을 생성했다. 1차원 배열과 Linked List를 결합하여 길이 101의 배열을 x좌표로 놓고, 각 배열안에 Linked List를 해당 x좌표에 있는 y값들로 초기화 했다.
 그리고 매번 점들 사이에 이동 가능한 **가용 연료량**를 최대 연료량을 100으로 나눈 연료량으로 한정했다. 다만, 한번 이동할 때 연료량이 남을 경우 그 남는 연료량까지 다음 이동시에 이용 할 수 있게 했다.
 예비 목적지와의 거리가 가용 연료량 이내이면서, 예비 목적지를 거쳐 (100, 100)까지 가는 거리가 앞으로 사용 가능한 총 연료량 이내일 경우에 예비 목적지로 이동하게 했다.