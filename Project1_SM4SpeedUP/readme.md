## SM4的代码实现及加速尝试
### 代码构成
```
Project1_SM4SpeedUP/
├── TrivalSM4.cpp                   # 基本的SM4实现
├── TTableSM4.cpp                   # 利用T-Table提高运行效率
├── TTable_SIMD_SM4.cpp             # 在T-table基础上应用SIMD优化
└── readme.md                       # 本说明文档
```
### 实验结果
| | | |
| :-: | :-: | :-: |
| ![Trivial.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project1_SM4SpeedUP/Trivial.png) | ![TTable.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project1_SM4SpeedUP/TTable.png) | ![TTable_SIMD.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project1_SM4SpeedUP/Ttable_SIMD.png) |
| 原始实现的性能截图 | T-table实现的性能截图 | T-table-SIMD实现的性能截图 |
| 4.96(6.32)MB/s | 35.61(36.85)MB/s | 39.74(40.46)MB/s |
| 原始实现的吞吐量 | T-table实现的吞吐量 | T-table-SIMD实现的吞吐量 |