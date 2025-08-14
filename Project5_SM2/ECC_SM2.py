from ecc.curve import PSM2 , Point , Curve
import random
import gmpy2

# 一些参数设置
ID_A = '1145141919810'  # 用户A的身份标识
paras = '''
p  = FFFFFFFE FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF 00000000 FFFFFFFF FFFFFFFF
a  = FFFFFFFE FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF 00000000 FFFFFFFF FFFFFFFC
b  = 28E9FA9E 9D9F5E34 4D5A9E4B CF6509A7 F39789F5 15AB8F92 DDBCBD41 4D940E93
n  = FFFFFFFE FFFFFFFF FFFFFFFF FFFFFFFF 7203DF6B 21C6052B 53BBF409 39D54123
Gx = 32C4AE2C 1F198119 5F990446 6A39C994 8FE30BBF F2660BE1 715A4589 334C74C7
Gy = BC3736A2 F4F6779C 59BDCEE3 6B692153 D0A9877C C62A4740 02DF32E5 2139F0A0
''' # 部分曲线参数
M = "Today is a good day" # 明文

def generate_keypair(curve: Curve = PSM2):
    """
    生成SM2密钥对
    :param curve: 使用的椭圆曲线，默认为PSM2
    :return: (私钥, 公钥)
    """
    private_key = random.randint(1, curve.n - 1)
    public_key = curve.mul_point(private_key, curve.G)
    return private_key, public_key

def sign(message, private_key, curve : Curve = PSM2):
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
    return (r, s)

def verify(signature, message, public_key, curve: Curve = PSM2):
    """
    验证签名
    :param signature: 签名元组(r, s)
    :param message: 消息哈希值
    :param public_key: 公钥点
    :param curve: 使用的椭圆曲线，默认为PSM2
    :return: 验证结果 True/False
    """
    r, s = signature
    # 验证签名有效
    if not (1 <= r < curve.n and 1 <= s < curve.n):
        return False
    t = (s + r) % curve.n
    if t == 0:
        return False
    A = s * curve.G + t * public_key
    if A.is_at_infinity():
        return False
    x1, y1 = A.x, A.y
    R = (message + x1) % curve.n
    return R == r
    

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
    signature = sign(e, private_key, PSM2)
    print(f"签名: {signature}")

    # B 收到 M, 计算出ZA和e, 验证签名
    if verify(signature, e, public_key, PSM2):
        print("签名验证通过")
    else:
        print("签名验证失败")
        raise ValueError("签名验证失败")