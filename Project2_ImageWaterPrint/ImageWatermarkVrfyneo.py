from BlindWatermark.BlindWatermark import watermark
from BlindWatermark.BlindWatermark import test_ncc
import argparse
import cv2
import numpy as np
import os

def get_ncc(filename1, filename2):
    # 复制自 ncc.py 的 test_ncc，但返回均值
    from BlindWatermark.BlindWatermark.ncc import NCC
    from BlindWatermark.BlindWatermark.tools import cv_imread
    a = cv_imread(filename1)
    b = cv_imread(filename2)
    nccs = []
    for i in range(3):
        nccs.append(NCC(a[:,:,i], b[:,:,i]))
    return np.mean(nccs)

def rotate_image(img_path, angle):
    img = cv2.imread(img_path)
    if angle == 90:
        img_rot = cv2.rotate(img, cv2.ROTATE_90_CLOCKWISE)
    elif angle == 180:
        img_rot = cv2.rotate(img, cv2.ROTATE_180)
    elif angle == 270:
        img_rot = cv2.rotate(img, cv2.ROTATE_90_COUNTERCLOCKWISE)
    else:
        img_rot = img
    tmp_path = f"{os.path.splitext(img_path)[0]}_rot{angle}.png"
    cv2.imwrite(tmp_path, img_rot)
    return tmp_path

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='生成图片水印')

    parser = argparse.ArgumentParser(usage="嵌入或者解出盲水印.", description="help info.")
    parser.add_argument("--key",'-k', default=(4399,2333,32),type=float, help="依次输入2个随机种子和除数(正数),除数可以是一个或者两个,之间用空格隔开.", dest="key",nargs='*')
    parser.add_argument('-d','--dwt_deep',default=1,type=int,help="设定小波变换的次数,次数增加会提高鲁棒性,但会减少图片承载水印的能力,通常取1,2,3",dest="dwt_deep")
    parser.add_argument('-bs','--block_shape',default=4,type=int,help='设定分块大小,因为限定长宽相同,所以只需要传一个整数就行了,对于大图可以使用更大的数,如8,更大的形状使得对原图影响更小,而且运算时间减少,但对鲁棒性没有提高,注意太大会使得水印信息超过图片的承载能力', dest="block_shape")
    parser.add_argument("--in_put",'-i', help="要嵌入或提取水印的图片的路径", dest="ori_img")
    parser.add_argument("--read_wm",'-wm', help="要嵌入的水印的路径", dest="wm")
    parser.add_argument("--wm_shape",'-ws', help="要解出水印的形状", dest="wm_shape",nargs=2)
    parser.add_argument("--out_put",'-o', help="图片的输出路径", dest="output")
    parser.add_argument("--show_ncc",'-s', help="展示输出图片和原图的NC值(相似度)", default=False, action="store_true", dest="show_ncc") 
    args = parser.parse_args()
    
    # 提取水印
    try:
        wm_shape0,wm_shape1 = args.wm_shape
        wm_shape0,wm_shape1 = int(wm_shape0),int(wm_shape1)
    except Exception as e:
        print("输入的水印形状",args.wm_shape,"不符合规定")
        print(e)
        exit()
    if len(args.key)==3:
        random_seed1,random_seed2,mod1 = args.key
        bwm = watermark(int(random_seed1),int(random_seed2),mod1,wm_shape=(wm_shape0,wm_shape1),block_shape = (args.block_shape,args.block_shape),dwt_deep=args.dwt_deep)
    elif len(args.key)==4:  
        random_seed1,random_seed2,mod1,mod2 = args.key
        bwm = watermark(int(random_seed1),int(random_seed2),mod1,mod2,wm_shape=(wm_shape0,wm_shape1),block_shape = (args.block_shape,args.block_shape),dwt_deep=args.dwt_deep)

    # 旋转角度列表
    angles = [0, 90, 180, 270]
    best_ncc = -1
    best_output = args.output
    best_angle = 0

    for angle in angles:
        if angle == 0:
            img_path = args.ori_img
        else:
            img_path = rotate_image(args.ori_img, angle)
        output_path = f"{os.path.splitext(args.output)[0]}_rot{angle}.png"
        bwm.extract(img_path, output_path)
        if args.show_ncc and args.wm:
            ncc = get_ncc(args.wm, output_path)
            print(f"角度 {angle}° 提取水印 NC值: {ncc}")
            if ncc > best_ncc:
                best_ncc = ncc
                best_output = output_path
                best_angle = angle

    print(f"最佳结果为旋转 {best_angle}°，输出路径: {best_output}，NC值: {best_ncc}")

    # 如果需要最终输出文件名为用户指定的，复制最佳结果
    if best_output != args.output:
        import shutil
        shutil.copy(best_output, args.output)