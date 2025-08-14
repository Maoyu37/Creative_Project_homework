from ecc.curve import PSM2 , Point , Curve
import random

# 我们仍采用初始加密模式，只改变签名算法定义
from ECC_SM2 import ID_A, paras, M, generate_keypair

def badsign(message, private_key, curve : Curve = PSM2):
    '''与正常签名算法一致，但泄露了随机数k'''
    k, kG, x1, y1, r, s = None, None, None, None, None, None
    while(True):
        # 生成签名用随机数
        k = random.randint(1, curve.n - 1)
        # 计算(x1,y1)=[k]G
        kG = k * curve.G
        x1 , y1 = kG.x , kG.y
        r = (message + x1) % curve.n
        if r==0 or r + k == curve.n:
            continue
        s = pow((1 + private_key), -1, curve.n) * (k - r * private_key) % curve.n
        if s == 0:
            continue
        else:
            break
    # 返回签名(r,s)
    return (r, s) , k

def attack(signature, k, curve: Curve = PSM2):
    r, s = signature
    d = pow(s + r, -1, curve.n) * (k - s) % curve.n
    return d

if __name__ == "__main__":
    # 生成密钥对
    private_key, public_key = generate_keypair()
    print(f"私钥: {private_key}")
    print(f"公钥: {public_key}")
    # 验证公钥是否在曲线上
    if PSM2.is_on_curve(public_key):
        print("公钥验证通过，公钥在曲线上")
    else:
        print("公钥验证失败，公钥不在曲线上")
        raise ValueError("公钥不在曲线上")
    # 计算ZA并生成消息M的哈希值e
    ZA = hash(ID_A + paras + str(public_key.x)) # 生产环境下的ZA见标准细节，这里仅作演示
    e = hash(str(ZA) + M)
    signature , k = badsign(e, private_key, PSM2)
    print(f"签名: {signature}")

    # B 得到签名(r,s)与k后，可以恢复A的私钥
    recovered_private_key = attack(signature, k, PSM2)
    print("攻击者恢复的私钥：",recovered_private_key)
    if(recovered_private_key == private_key):
        print("攻击成功！")