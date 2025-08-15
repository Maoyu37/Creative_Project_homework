#include <iostream>
#include <string>
#include <cmath>
#include <cstdint>  // 添加uint32_t支持
#include <chrono> 
using namespace std;

#define DEBUG_MODE false

// 使用uint32_t替代bytes数组
typedef uint32_t word;

// 初始向量 (大端序存储)
word IV[8] = {
    0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600,
    0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E
};

// 循环左移 (直接操作32位整数)
inline word ROTL(word x, int n) {
    return (x << n) | (x >> (32 - n));
}

// P置换优化
inline word P0(word x) {
    return x ^ ROTL(x, 9) ^ ROTL(x, 17);
}

inline word P1(word x) {
    return x ^ ROTL(x, 15) ^ ROTL(x, 23);
}

// 布尔函数优化 (直接操作32位整数)
inline word FF(word x, word y, word z, int j) {
    return (j < 16) ? (x ^ y ^ z) : ((x & y) | (x & z) | (y & z));
}

inline word GG(word x, word y, word z, int j) {
    return (j < 16) ? (x ^ y ^ z) : ((x & y) | ((~x) & z));
}

// 常量T直接定义为32位整数
inline word T(int j) {
    return (j < 16) ? 0x79CC4519 : 0x7A879D8A;
}

// 输入和填充优化
int Input_and_Pad(uint8_t* padded_input) {
    string input_hex;
    cin >> input_hex;

    // 转换为字节数组
    int input_len = input_hex.length() / 2;
    for (int i = 0; i < input_len; ++i) {
        string byte_str = input_hex.substr(2 * i, 2);
        padded_input[i] = static_cast<uint8_t>(strtol(byte_str.c_str(), nullptr, 16));
    }

    // 计算填充长度
    uint64_t bit_len = input_len * 8ULL;
    padded_input[input_len] = 0x80;

    int pad_len = 55 - (input_len % 64);  // 直接计算填充长度
    if (pad_len < 0) pad_len += 64;

    // 添加长度信息 (大端序)
    for (int i = 0; i < 8; ++i) {
        padded_input[input_len + 1 + pad_len + i] = static_cast<uint8_t>(bit_len >> (56 - 8 * i));
    }

    return input_len + 1 + pad_len + 8;
}

// 消息扩展优化 (使用uint32_t)
void extend_message(const uint8_t* block, word* W) {
    // 大端序加载
    for (int i = 0; i < 16; ++i) {
        W[i] = (static_cast<word>(block[i * 4]) << 24) |
            (static_cast<word>(block[i * 4 + 1]) << 16) |
            (static_cast<word>(block[i * 4 + 2]) << 8) |
            static_cast<word>(block[i * 4 + 3]);
    }

    // 循环展开 (每次处理4个)
    for (int i = 16; i < 68; i += 4) {
        W[i] = P1(W[i - 16] ^ W[i - 9] ^ ROTL(W[i - 3], 15))
            ^ ROTL(W[i - 13], 7) ^ W[i - 6];

        if (i + 1 < 68) W[i + 1] = P1(W[i - 15] ^ W[i - 8] ^ ROTL(W[i - 2], 15))
            ^ ROTL(W[i - 12], 7) ^ W[i - 5];

        if (i + 2 < 68) W[i + 2] = P1(W[i - 14] ^ W[i - 7] ^ ROTL(W[i - 1], 15))
            ^ ROTL(W[i - 11], 7) ^ W[i - 4];

        if (i + 3 < 68) W[i + 3] = P1(W[i - 13] ^ W[i - 6] ^ ROTL(W[i], 15))
            ^ ROTL(W[i - 10], 7) ^ W[i - 3];
    }
}

// 压缩函数优化 (直接使用uint32_t)
void CF(word* V, const uint8_t* block) {
    word W[68], WW[64];
    extend_message(block, W);

    // 生成WW
    for (int i = 0; i < 64; i++) {
        WW[i] = W[i] ^ W[i + 4];
    }

    // 初始化状态寄存器
    word A = V[0], B = V[1], C = V[2], D = V[3];
    word E = V[4], F = V[5], G = V[6], H = V[7];

    // 主循环 (展开4次)
    for (int j = 0; j < 64; j += 4) {
        // 第一轮
        word SS1 = ROTL(ROTL(A, 12) + E + ROTL(T(j), j), 7);
        word SS2 = SS1 ^ ROTL(A, 12);
        word TT1 = FF(A, B, C, j) + D + SS2 + WW[j];
        word TT2 = GG(E, F, G, j) + H + SS1 + W[j];
        D = C;
        C = ROTL(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTL(F, 19);
        F = E;
        E = P0(TT2);

        // 第二轮 (j+1)
        if (j + 1 < 64) {
            SS1 = ROTL(ROTL(A, 12) + E + ROTL(T(j + 1), j + 1), 7);
            SS2 = SS1 ^ ROTL(A, 12);
            TT1 = FF(A, B, C, j + 1) + D + SS2 + WW[j + 1];
            TT2 = GG(E, F, G, j + 1) + H + SS1 + W[j + 1];
            D = C;
            C = ROTL(B, 9);
            B = A;
            A = TT1;
            H = G;
            G = ROTL(F, 19);
            F = E;
            E = P0(TT2);
        }

        // 第三轮 (j+2)
        if (j + 2 < 64) {
            SS1 = ROTL(ROTL(A, 12) + E + ROTL(T(j + 2), j + 2), 7);
            SS2 = SS1 ^ ROTL(A, 12);
            TT1 = FF(A, B, C, j + 2) + D + SS2 + WW[j + 2];
            TT2 = GG(E, F, G, j + 2) + H + SS1 + W[j + 2];
            D = C;
            C = ROTL(B, 9);
            B = A;
            A = TT1;
            H = G;
            G = ROTL(F, 19);
            F = E;
            E = P0(TT2);
        }

        // 第四轮 (j+3)
        if (j + 3 < 64) {
            SS1 = ROTL(ROTL(A, 12) + E + ROTL(T(j + 3), j + 3), 7);
            SS2 = SS1 ^ ROTL(A, 12);
            TT1 = FF(A, B, C, j + 3) + D + SS2 + WW[j + 3];
            TT2 = GG(E, F, G, j + 3) + H + SS1 + W[j + 3];
            D = C;
            C = ROTL(B, 9);
            B = A;
            A = TT1;
            H = G;
            G = ROTL(F, 19);
            F = E;
            E = P0(TT2);
        }
    }

    // 更新状态
    V[0] ^= A; V[1] ^= B; V[2] ^= C; V[3] ^= D;
    V[4] ^= E; V[5] ^= F; V[6] ^= G; V[7] ^= H;
}

int main() {
    uint8_t padded_input[65536] = { 0 };
    int padded_len = Input_and_Pad(padded_input);
    int num_blocks = padded_len / 64;

    word V[8];
    for (int i = 0; i < 8; i++) {
        V[i] = IV[i];
    }

    // 性能测试参数
    const int test_rounds = 10000;
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    for (int t = 0; t < test_rounds; ++t) {
        // 每轮都重置V
        word Vtest[8];
        for (int i = 0; i < 8; i++) Vtest[i] = IV[i];
        for (int i = 0; i < num_blocks; i++) {
            CF(Vtest, padded_input + i * 64);
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    double avg_time = duration.count() / static_cast<double>(test_rounds);

    cout << "SM3 Hash: ";
    for (int i = 0; i < 8; i++) {
        cout << hex << ((V[i] >> 24) & 0xFF)
            << hex << ((V[i] >> 16) & 0xFF)
            << hex << ((V[i] >> 8) & 0xFF)
            << hex << (V[i] & 0xFF);
    }
    cout << endl;

    cout << "SM3 性能测试: " << test_rounds << " 次平均耗时 " << avg_time << " 微秒" << endl;

    return 0;
}