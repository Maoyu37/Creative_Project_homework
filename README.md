# 📌 2025创新作业实践课程作业展示

**项目说明**：本项目包含了简单的作业课题介绍，代码简要说明及部分代码的运行展示。部分项目可能只展示代码及运行结果，需要用户自行进行本地部署适配等工作。

## Project1 SM4的代码实现及加速尝试
- TrivalSM4.cpp 文件包含了SM4的C++实现，以及一个简单的benchmark
- TTableSM4.cpp 文件包含了上述实现的TTable加速版本，实现了约6倍性能提升
- TTable_SIMD_SM4.cpp 文件利用smmintrin实现了SIMD向量化并行优化，进一步改善了性能
- 实验截图见Project1文件夹

## Project2 图片水印嵌入提取的算法优化
该任务利用了开源脚本[BlindWatermark](https://github.com/fire-keeper/BlindWatermark)给出的基于离散小波变换的水印嵌入提取算法。
我对该脚本组件进行了重新封装，形成了用于给图片添加水印的ImageWatermarkGen.py与从图片提取水印的ImageWatermarkVrfy.py。
- ImageWatermarkVrfyneo.py 在上述代码的基础上解决了原算法在被旋转后水印失效的问题
- readme.md中包含了对上述项目的黑白、彩色版本图片添加或删除复杂水印图像的效果测试，对旋转、对比度调整等常见图像变换做了鲁棒性检测
- 对于进一步的抵抗裁剪、拼贴、平移、遮挡、噪点等攻击的表现，建议读者查阅[BlindWatermark给出的测试结果](https://github.com/fire-keeper/BlindWatermark/blob/master/readme.md)

## Project3 Poseidon2哈希算法的circom实现
本任务利用[circom2](https://github.com/iden3/circom)给出的平台编写实现了[Poseidon2](https://eprint.iacr.org/2023/323.pdf)哈希算法，并将其编译为了ZKP常用的R1CS形式文件。
为了验证证明正确性，需要事先计算若干组Poseidon2的原像-哈希对，实验中采用了[HorizonLabs给出的开源Poseidon2实现](https://github.com/HorizenLabs/poseidon2/tree/main)预计算出若干原像-哈希对，见项目文件夹中的input.json
- poseidon2.circom为本项目实现的Poseidon2电路
- input.json为线路评估时的输入变量
- test.py用于将开源Poseidon2实现的rust项目给出的大端序BIG_INT输出转换为十进制大整数

## Project4 SM3的软件实现与优化
- MySM3.cpp给出了SM3杂凑算法的C++实现，经过[国家密码管理局给出的测试样例](https://oscca.gov.cn/sca/xxgk/2010-12/17/1002389/files/302a3ada057c4a73830536d03e683110.pdf)验证无误


## 开源许可
由于部分代码继承自其他项目，本项目采用GPL-3.0 license

## 第三方资源

本项目使用了以下开源资源：

### [BlindWatermark](https://github.com/fire-keeper/BlindWatermark)
- **作者**: [fire-keeper](https://github.com/fire-keeper)
- **许可证**: [GPL-3.0 license](https://github.com/fire-keeper/BlindWatermark/blob/master/LICENSE))
- **用途**: 作为Project2的基础进行二次开发
