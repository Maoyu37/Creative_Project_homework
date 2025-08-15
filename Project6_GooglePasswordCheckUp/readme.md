# 基于离散对数的密码安全检查系统
## 代码构成
```
Project6_GooglePasswordCheckUp/
├── UserInfo.py.                    # 存储用户名-密码对用于校验代码准确性
├── GooglePasswordCheckup.py        # 校验算法的POC
├── Realworld_Implementation.py     # 根据论文还原的实际实现算法
└── readme.md                       # 本说明文档
```
## POC实现
    这个项目实现了一个类似Google Password Checkup的隐私保护协议，允许用户在不暴露实际密码的情况下，检查自己的用户名-密码对是否存在于服务器的数据库中。系统基于离散对数问题的数学原理，确保用户隐私得到保护。
### 数学原理
    系统使用离散对数问题的困难性来保护用户隐私：
    - 服务器存储密码的加密形式vi = hi^b mod n，其中hi = hash(username||password)
    - 用户查询时发送临时密钥加密的密码v = h^a mod n
    - 通过双线性映射验证密码存在性而不暴露原始值
### 实验结果
    ![POC_result.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project6_GooglePasswordCheckUp/POC_result.png)

## 完整协议实现
    这一版本的实现完整重现了[谷歌团队的论文](https://eprint.iacr.org/2019/723.pdf)中section 3.1一节Figure 2中展示的协议