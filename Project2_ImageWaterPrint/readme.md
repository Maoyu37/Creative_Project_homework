## 图片水印实验结果
### 用于测试的原始图片及黑白水印
| | |
| :-: | :-: |
| ![lena.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/lena.png) | ![lena_grey.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/lena_grey.png) |
| lena.png | lena_grey.png |

| |
| :-: |
| ![watermark.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/wm.png) |
| 水印图片 |
### 直接添加水印后，图片会受到轻微的影响，但ncc值依然保持在0.998以上，说明添加水印不影响图片正常传播
| | |
| :-: | :-: |
| ![lena_output.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/lena_output.png) | ![lena_grey_output.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/lena_grey_output.png) |
| lena_watermarked.png | lena_grey_watermarked.png |
| averagencc:0.998 | averagencc:0.999 |
| ![CLI1.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/generatecoloredwatermark.png) | ![CLI2.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/generatewatermark.png) |
### 直接提取水印，观察到水印图片有一定失真，但整体能够分辨，ncc值高于0.95
| | |
| :-: | :-: |
| ![lena_out_wm.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/lena_out_wm.png) | ![out_wm.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/out_wm.png) |
| watermark | grey_watermark |
| averagencc:0.959 | averagencc:0.960 |
| ![CLI1.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/verifycoloredwatermark.png) | ![CLI2.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/verifywatermark.png) |
### 将图片旋转270°，利用ImageWatermarkVrfyneo.py尝试提取旋转后的图片水印，发现水印依然能够识别
| | |
| :-: | :-: |
| ![lena_out_wm_swing.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/lena_out_wm_swing.png) | ![out_wm_swing.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/out_wm_swing.png) |
| watermark | grey_watermark |
| averagencc:0.959 | averagencc:0.960 |
| ![swing_CLI1.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/swing_CLI1.png) | ![swing_CLI2.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/swing_CLI2.png) |
### 调整对比度会对结果产生很大的影响，如下表所示，黑白图像在提高20%对比度的情况下依然能够大致辨认水印，但彩色图像提高20%对比度后不可识别。  
### 此外，需要注意失真率并不与对比度成正比或反比，如下表中黑白图像提高10%对比度损失的精度高于提高20%对比度
| | |
| :-: | :-: |
| ![lena_out_wm_cr10.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/lena_out_wm_cr10.png) | ![out_wm_cr10.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/out_wm_cr10.png) |
| Contrast ratio +10% | Contrast ratio +10% |
| averagencc:0.424 | averagencc:0.810 |
| ![cr10_CLI1.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/cr10_CLI1.png) | ![cr10_CLI2.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/cr10_CLI2.png) |
| ![lena_out_wm_cr20.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/lena_out_wm_cr20.png) | ![out_wm_cr20.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/out_wm_cr20.png) |
| Contrast ratio +20% | Contrast ratio +20% |
| averagencc:-0.057 | averagencc:0.820 |
| ![cr20_CLI1.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/cr20_CLI1.png) | ![cr20_CLI2.png](https://github.com/Maoyu37/Creative_Project_homework/blob/main/Project2_ImageWaterPrint/cr20_CLI2.png) |
### 该算法抵抗裁剪、平移、压缩、覆盖、噪点的性能都较好，详见[作者本人的测试文档](https://github.com/fire-keeper/BlindWatermark/blob/master/readme.md)