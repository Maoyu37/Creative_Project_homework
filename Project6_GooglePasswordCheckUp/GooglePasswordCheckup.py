import random
from UserInfo import credentials # 用户名-密码对太长了，不适合放在主文件夹里

# 虽然不确定需不需要，但总之先生成一个1000位大素数备用
n = 27159907518918494018855808314686718163691760352686040062816457522438905384105285471233896451092862038466615137001705198682414412284187320729962646453110706096594443839212718307257477492041755189136102429185342198349345518217980253860794527207589042170167501242312405645714627527456502240185457435011575981309


KV_table = {}

def Process_data(usernames, passwords, b):
    h_i = hash(usernames + passwords) % n
    hi_bytes = str(h_i).encode('utf-8')
    key_i = int.from_bytes(hi_bytes[:2], 'big') % n
    v_i = pow(h_i, b, n)
    # if usernames == "user1":
    #     print(f"用户名: {usernames}, 密码: {passwords}, 哈希值: {h_i}, k: {key_i}, v = h^b: {v_i}")
    return (key_i , v_i)

def gen_k_v(usernames, passwords):
    # generate temporary secret key
    flag = False
    a = None
    while(not flag):
        try:
            a = random.randint(1, n - 2) # a in [1, phi(n) - 1]
            a_inv = pow(a, -1, n - 1) # a_inv = a^(-1) mod phi(n)
            flag = True
        except ValueError:
            pass
    h = hash(usernames + passwords) % n
    # 取str(h)的前2字节
    h_bytes = str(h).encode('utf-8')
    key = int.from_bytes(h_bytes[:2], 'big') % n
    v = pow(h, a, n)
    # if usernames == "user1":
    #     print(f"用户名: {usernames}, 密码: {passwords}, 哈希值: {h}, k: {key}, v=h^a: {v}")
    return (key , v) , a


if __name__ == "__main__":
    # generate server secret key
    b = random.randint(1, n - 1)
    # 用户在服务器端注册他们的username与password(当然也许是加盐哈希形式)
    for username, password in credentials.items():
        ki , vi = Process_data(username, password, b)
        if ki in KV_table:
            KV_table[ki].append(vi)
        else:
            KV_table[ki] = []
            KV_table[ki].append(vi)
    
    # 用户利用自己的短期密钥生成(key, v),询问服务器
    # 以"user1": "xK3@9fLq!"为例
    kv_pairs , a = gen_k_v("user1", "xK3@9fLq!")
    # 用户将kv_pairs发送给服务器

    # 服务器计算h^ab
    k, v = kv_pairs
    h_pow_ab = pow(v, b, n)
    # 找到集合S
    S = KV_table[k]
    # 服务器将h^ab, S发送给用户

    # 用户计算h^b
    Inv_a = pow(a, -1, n - 1)  # a的逆元
    h_pow_b = pow(h_pow_ab, Inv_a, n)
    if h_pow_b in S:
        print("该用户名-密码对在服务器上存在！")
    else:
        print("该用户名-密码对不存在！")    
