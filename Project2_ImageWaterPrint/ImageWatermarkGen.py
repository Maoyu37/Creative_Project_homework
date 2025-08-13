from BlindWatermark.BlindWatermark import watermark
from BlindWatermark.BlindWatermark import test_ncc
import argparse

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
    
    #嵌入水印
    if len(args.key)==3:
        random_seed1,random_seed2,mod1 = args.key
        bwm = watermark(int(random_seed1),int(random_seed2),mod1,block_shape = (args.block_shape,args.block_shape),dwt_deep=args.dwt_deep)
    elif len(args.key)==4:  
        random_seed1,random_seed2,mod1,mod2 = args.key
        bwm = watermark(int(random_seed1),int(random_seed2),mod1,mod2,block_shape = (args.block_shape,args.block_shape),dwt_deep=args.dwt_deep)
    else:
        print("您输入了{}个key,但是本程序现在只支持3个或者4个key".format(len(args.key)))
        exit()

    bwm.read_ori_img(args.ori_img)
    bwm.read_wm(args.wm)
    bwm.embed(args.output)
    if args.show_ncc:
        test_ncc(args.ori_img,args.output)

