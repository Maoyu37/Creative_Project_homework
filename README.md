# 📌 2025创新作业实践课程作业展示

**项目说明**：本项目包含了简单的作业课题介绍，代码简要说明及部分代码的运行展示。部分(特别指c++源码编写的)项目可能只展示代码及运行结果，需要用户自行进行本地部署等工作。

## Project1 SM4的代码实现及加速尝试
- TrivalSM4.cpp 文件包含了SM4的C++实现，以及一个简单的benchmark
- TTableSM4.cpp 文件包含了上述实现的TTable加速版本，实现了约6倍性能提升
- TTable_SIMD_SM4.cpp 文件利用smmintrin实现了SIMD向量化并行优化，进一步改善了性能
- 实验截图见Project1文件夹,详细数据记录在[readme.md](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project1_SM4SpeedUP/readme.md)中。

## Project2 图片水印嵌入提取的算法优化
该任务利用了开源脚本[BlindWatermark](https://github.com/fire-keeper/BlindWatermark)给出的基于离散小波变换的水印嵌入提取算法。
我对该脚本组件进行了重新封装，形成了用于给图片添加水印的ImageWatermarkGen.py与从图片提取水印的ImageWatermarkVrfy.py。
- ImageWatermarkVrfyneo.py 在上述代码的基础上解决了原算法在被旋转后水印失效的问题
- [readme.md](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/readme.md)中包含了对上述项目的黑白、彩色版本图片添加或删除复杂水印图像的效果测试，对旋转、对比度调整等常见图像变换做了鲁棒性检测
- 对于进一步的抵抗裁剪、拼贴、平移、遮挡、噪点等攻击的表现，建议读者查阅[BlindWatermark给出的测试结果](https://github.com/fire-keeper/BlindWatermark/blob/master/readme.md)

## Project3 Poseidon2哈希算法的circom实现
本任务利用[circom2](https://github.com/iden3/circom)给出的平台编写实现了[Poseidon2](https://eprint.iacr.org/2023/323.pdf)哈希算法，并将其编译为了ZKP常用的R1CS形式文件。
为了验证证明正确性，需要事先计算若干组Poseidon2的原像-哈希对，实验中采用了[HorizonLabs给出的开源Poseidon2实现](https://github.com/HorizenLabs/poseidon2/tree/main)预计算出若干原像-哈希对，见项目文件夹中的input.json
- poseidon2.circom为本项目实现的Poseidon2电路
- input.json为线路评估时的输入变量
- test.py用于将开源Poseidon2实现的rust项目给出的大端序BIG_INT输出转换为十进制大整数
- poseidon2.r1cs为用于零知识证明的约束文件
- witness.wtns为见证者文件

## Project4 SM3的软件实现与优化
- MySM3.cpp给出了SM3杂凑算法的C++实现，经过[国家密码管理局给出的测试样例](https://oscca.gov.cn/sca/xxgk/2010-12/17/1002389/files/302a3ada057c4a73830536d03e683110.pdf)验证无误
- uint32.cpp给出了基于类型优化、循环展开等的优化版本，思路与SM4类似。
- [readme.md](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project4_SM3SpeedUP/readme.md)给出了一个长度扩展攻击思路，LengthAttendAttack1.cpp实现了其中关键步骤的代码

## Project5 SM2
实现中采用的椭圆曲线算法库来自开源项目[ecc-pycrypto](https://github.com/lc6chang/ecc-pycrypto)
- 对该库中curve模块进行了二次开发，使其支持GM/T 0003.5-2012 规范中定义的SM2曲线的相关运算
- 基于上述修改结果实现了SM2的签名验签算法ECC_SM2.py
- 在ECC_SM2.py及课件的基础上实现了若干签名算法的误用攻击
    -- SM2_leaking_k_attack.py给出加密随机数k泄露时恢复私钥的攻击
    -- SM2_reuse_k_attack.py给出同一随机数被用于多次签名时恢复私钥的攻击
    -- 课件指出两个不同角色采用相同的随机数进行签名会将自己的私钥互相泄露出去，但这本质上就是SM2_leaking_k_attack.py实现的攻击，本项目不再展示
- [readme.md](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project5_SM2/readme.md)给出了具体代码结构及项目描述

## Project6 GooglePasswordCheckUp
这个项目实现了一个类似Google Password Checkup的隐私保护协议，允许用户在不暴露实际密码的情况下，检查自己的用户名-密码对是否存在于服务器的数据库中。系统基于离散对数问题的学原理，确保用户隐私得到保护。
- UserInfo.py 保存了100个用户名-密码对用于测试协议是否运转正常
- GooglePasswordCheckup.py 参考自PPT上的协议描述，实现了POC的用户查询机制。
- PaperImplementation.py给出了参照[谷歌团队的论文](https://eprint.iacr.org/2019/723.pdf)中section 3.1一节Figure 2中展示的协议。
- [readme.md](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project6_GooglePasswordCheckUp/readme.md)记录了算法数学原理，项目构成及实验结果

## 开源许可
由于部分代码继承自其他项目，本项目采用GPL-3.0 license

## 第三方资源 & 致谢

本项目使用了以下开源资源，感谢以下开源创作者的精彩工作：

### [BlindWatermark](https://github.com/fire-keeper/BlindWatermark)
- **作者**: [fire-keeper](https://github.com/fire-keeper)
- **许可证**: [GPL-3.0 license](https://github.com/fire-keeper/BlindWatermark/blob/master/LICENSE)
- **用途**: 作为Project2的基础进行二次开发

### [circom2](https://github.com/iden3/circom)
- **作者**: [iden3](https://github.com/iden3)
- **许可证**: [GPL-3.0 license](https://github.com/iden3/circom?tab=GPL-3.0-1-ov-file)
- **用途**: 提供零知识证明电路的语法与编译平台支持，用于生成相关约束文件及witness

### [poseidon2](https://github.com/HorizenLabs/poseidon2/tree/main)
- **作者**: [HorizenLabs](https://github.com/HorizenLabs)
- **许可证**: [Apache License 2.0](https://github.com/HorizenLabs/poseidon2/blob/main/LICENSE-APACHE), [MIT license](https://github.com/HorizenLabs/poseidon2/blob/main/LICENSE-MIT)
- **用途**: 作为Poseidon2算法的rust开源实现，我们在Project3的电路借鉴了其代码思路，且用这一开源算法评估了若干标准输入输出是否正确

### [ecc-pycrypto](https://github.com/lc6chang/ecc-pycrypto)
- **作者**: [lc6chang](https://github.com/lc6chang)
- **许可证**: [MIT license](https://github.com/lc6chang/ecc-pycrypto?tab=MIT-1-ov-file#readme)
- **用途**: 作为Project5中SM2的底层椭圆曲线算法库，实现了SM2签名算法的数学基础组件

### [python-paillier](https://github.com/data61/python-paillier)
- **作者**: [data61](https://github.com/data61)
- **许可证**: [GPL-3.0 license](https://github.com/data61/python-paillier/blob/master/LICENSE.txt)
- **用途**: 用于实现Project6中的安全交集协议，提供python上的Paillier加法同态加密支持