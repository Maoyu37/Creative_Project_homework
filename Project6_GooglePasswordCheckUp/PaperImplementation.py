import random
import hashlib
from phe import paillier  # 加法同态加密库

class Party1:
    """P1：持有标识符集合V"""
    def __init__(self, V):
        self.V = V  # 标识符集合
        self.k1 = None  # 私钥
        self.intersection_size = 0  # 交集大小

    def round1(self, G):
        """第一轮：计算并发送 {H(v_i)^k1}"""
        self.k1 = random.randint(1, G.q-1)  # 随机选择k1
        hashed_exponents = []
        for v in self.V:
            # 计算 H(v) = g^{hash(v)}
            h = pow(G.g, int(hashlib.sha256(v.encode()).hexdigest(), 16) % G.p, G.q)
            # 计算 H(v)^k1 mod p
            hashed_exponents.append(pow(h, self.k1, G.p))
        random.shuffle(hashed_exponents)  # 随机打乱顺序
        return hashed_exponents

    def round3(self, A_set, B_set, pk):
        """第三轮：计算交集和求和"""
        # 计算 {H(w_j)^(k1*k2)}
        B_hashed = [
            pow(cj, self.k1, G.p) for (cj, _) in B_set
        ]
        
        # 寻找交集并求和
        intersection_enc_sum = pk.encrypt(0)  # Paillier加密的0
        for a in A_set:
            if a in B_hashed:
                idx = B_hashed.index(a)
                enc_tj = B_set[idx][1]
                # 同态加法：累加加密值
                intersection_enc_sum = intersection_enc_sum + enc_tj
                self.intersection_size += 1
        return intersection_enc_sum, self.intersection_size

class Party2:
    """P2：持有标识符-值对集合W"""
    def __init__(self, W):
        self.W = W  # 集合{(w_j, t_j)}
        self.k2 = None  # 私钥
        self.pk, self.sk = paillier.generate_paillier_keypair()  # 同态加密密钥

    def round2(self, A_received, G):
        """第二轮：计算并发送两个集合"""
        self.k2 = random.randint(1, G.q-1)  # 随机选择k2
        
        # 计算 {H(v_i)^(k1*k2)}
        A_set = [pow(a, self.k2, G.p) for a in A_received]
        random.shuffle(A_set)  # 随机打乱
        
        # 计算 {(H(w_j)^k2, Enc(t_j))}
        B_set = []
        for (w, t) in self.W:
            # 计算 H(w_j) = g^{hash(w_j)}
            h = pow(G.g, int(hashlib.sha256(w.encode()).hexdigest(), 16) % G.p, G.q)
            # 计算 H(w_j)^k2 mod p
            cj = pow(h, self.k2, G.p)
            # 加密 t_j
            enc_tj = self.pk.encrypt(t)
            B_set.append((cj, enc_tj))
        random.shuffle(B_set)  # 随机打乱
        
        return A_set, B_set

    def decrypt_sum(self, enc_sum):
        """解密交集的和"""
        return self.sk.decrypt(enc_sum)

class DHGroup:
    """Diffie-Hellman群参数"""
    def __init__(self, p, g, q):
        self.p = p  # 素数模
        self.g = g  # 生成元
        self.q = q  # 群阶

# 示例运行
if __name__ == "__main__":
    # 1. 设置DH群参数 (使用小素数便于演示)
    p = 1019   # 素数模
    g = 2      # 生成元
    q = 509    # 群阶 (p=2q+1)
    G = DHGroup(p, g, q)
    
    # 2. 初始化双方数据
    V = ["user1", "user2", "user3"]          # P1的标识符集合
    W = [("user2", 10), ("user3", 20), ("user4", 30)]  # P2的(标识符, 值)集合
    
    p1 = Party1(V)
    p2 = Party2(W)
    
    # 3. 协议执行
    # Round 1: P1 -> P2
    A1 = p1.round1(G)
    
    # Round 2: P2 -> P1
    A_set, B_set = p2.round2(A1, G)
    
    # Round 3: P1 -> P2
    enc_sum, cardinality = p1.round3(A_set, B_set, p2.pk)
    total_sum = p2.decrypt_sum(enc_sum)
    
    # 4. 输出结果
    print(f"交集大小 (Cardinality): {cardinality}")
    print(f"交集值和 (Sum): {total_sum}")
    print(f"P1本地输出: Cardinality = {p1.intersection_size}")