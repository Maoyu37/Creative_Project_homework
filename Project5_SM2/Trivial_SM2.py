import random
import hashlib

def generate_prime(bit_length):
    """生成大素数"""
    while True:
        p = random.getrandbits(bit_length)
        if p % 2 != 0 and all(p % i != 0 for i in range(3, int(p**0.5)+1, 2)):
            return p

def generate_parameters(bit_length=256):
    """生成SM2参数：素数p、阶n、生成元g"""
    p = generate_prime(bit_length)
    # 简化：假设n是p-1的一个大素因子，g是生成元
    n = p - 1  # 实际应用中n应为p-1的最大素因子，此处简化
    g = 2  # 通常选择2作为生成元
    return p, n, g

def generate_keys(p, g):
    """生成私钥d和公钥h"""
    d = random.randint(1, p-1)  # 私钥
    h = pow(g, d, p)            # 公钥
    return d, h

def sm2_sign(message, d, p, n, g):
    """SM2签名算法"""
    e = int(hashlib.sha256(message).hexdigest(), 16) % n  # 消息哈希
    
    while True:
        k = random.randint(1, n-1)  # 随机数k
        
        # 计算签名(r, s)
        x1 = pow(g, k, p)           # 模拟椭圆曲线点乘
        r = (e + x1) % n
        if r == 0 or r + k == n:    # 检查r有效性
            continue
            
        # 计算s = (k - r * d) * (1 + d)^{-1} mod n
        s = (k - r * d) * pow(1 + d, n-2, n) % n
        if s != 0:  # 检查s有效性
            return r, s

def sm2_verify(message, signature, h, p, n, g):
    """SM2验证算法"""
    r, s = signature
    e = int(hashlib.sha256(message).hexdigest(), 16) % n  # 消息哈希
    
    # 计算t = (r + s) mod n
    t = (r + s) % n
    if t == 0:
        return False
    
    # 验证 r = (e + (g^s * h^t mod p)) mod n
    x1_v = (pow(g, s, p) * pow(h, t, p)) % p
    r_v = (e + x1_v) % n
    return r_v == r

# ======================== 使用示例 ========================
if __name__ == "__main__":
    # 1. 生成系统参数
    p, n, g = generate_parameters(bit_length=256)
    print(f"系统参数:\n  p = {p}\n  n = {n}\n  g = {g}\n")
    
    # 2. 生成密钥对
    d, h = generate_keys(p, g)
    print(f"私钥 d = {d}")
    print(f"公钥 h = {h}\n")
    
    # 3. 签名
    message = b"Hello, SM2!"
    signature = sm2_sign(message, d, p, n, g)
    print(f"消息: {message.decode()}")
    print(f"签名: (r={signature[0]}, s={signature[1]})\n")
    
    # 4. 验证
    is_valid = sm2_verify(message, signature, h, p, n, g)
    print(f"验证结果: {'成功' if is_valid else '失败'}")
    
    # 5. 篡改消息测试
    tampered_message = b"Hello, SM3!"
    is_valid_tampered = sm2_verify(tampered_message, signature, h, p, n, g)
    print(f"篡改消息验证: {'成功' if is_valid_tampered else '失败'}")