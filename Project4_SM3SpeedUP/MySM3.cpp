#include <iostream>
#include <string>
#include <cmath>
using namespace std;
typedef unsigned char bytes;
#define DEBUG_MODE false

// 常数与函数定义等
string IV_string = "7380166F4914B2B9172442D7DA8A0600A96F30BC163138AAE38DEE4DB0FB0E4E";
bytes IV[8][4] = {
    {0x73, 0x80, 0x16, 0x6F},
    {0x49, 0x14, 0xB2, 0xB9},
    {0x17, 0x24, 0x42, 0xD7},
    {0xDA, 0x8A, 0x06, 0x00},
    {0xA9, 0x6F, 0x30, 0xBC},
    {0x16, 0x31, 0x38, 0xAA},
    {0xE3, 0x8D, 0xEE, 0x4D},
    {0xB0, 0xFB, 0x0E, 0x4E}
};

//   数据类型转换
unsigned int bytes_to_uint32(const bytes b[4]) {
    return (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
}
void uint32_to_bytes(unsigned int val, bytes out[4]) {
    out[0] = (val >> 24) & 0xFF;
    out[1] = (val >> 16) & 0xFF;
    out[2] = (val >> 8) & 0xFF;
    out[3] = val & 0xFF;
}

void circular_shiftleft_32bit(const bytes input[4], int dis, bytes out[4]) {
    unsigned int val = bytes_to_uint32(input);
    val = (val << dis) | (val >> (32 - dis));
    uint32_to_bytes(val, out);
}

void xor_32bit(const bytes input1[4], const bytes input2[4], bytes out[4]) {
    for (int i = 0; i < 4; ++i) {
        out[i] = input1[i] ^ input2[i];
    }
}

void and_32bit(const bytes input1[4], const bytes input2[4], bytes out[4]) {
    for (int i = 0; i < 4; ++i) {
        out[i] = input1[i] & input2[i];
    }
}

void or_32bit(const bytes input1[4], const bytes input2[4], bytes out[4]) {
    for (int i = 0; i < 4; ++i) {
        out[i] = input1[i] | input2[i];
    }
}

void not_32bit(const bytes input[4], bytes out[4]) {
    for (int i = 0; i < 4; ++i) {
        out[i] = ~input[i];
    }
}

void copy_32bit(const bytes src[4], bytes dest[4]) {
    for (int i = 0; i < 4; ++i) {
        dest[i] = src[i];
    }
}

void P_Permutation(const bytes X[4], bool type, bytes out[4]) {
    bytes tmp1[4], tmp2[4], tmp3[4];
    if (type == 0) {
        circular_shiftleft_32bit(X, 9, tmp1);
        circular_shiftleft_32bit(X, 17, tmp2);
        xor_32bit(tmp1, tmp2, tmp3);
        xor_32bit(X, tmp3, out);
    }
    else if (type == 1) {
        circular_shiftleft_32bit(X, 15, tmp1);
        circular_shiftleft_32bit(X, 23, tmp2);
        xor_32bit(tmp1, tmp2, tmp3);
        xor_32bit(X, tmp3, out);
    }
}

void xor_256bit(const bytes input1[8][4], const bytes input2[8][4], bytes out[8][4]) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 4; ++j) {
            out[i][j] = input1[i][j] ^ input2[i][j];
        }
    }
}

void add_32bit(const bytes input1[4], const bytes input2[4], bytes out[4]) {
    unsigned int val1 = bytes_to_uint32(input1);
    unsigned int val2 = bytes_to_uint32(input2);
    unsigned int sum = (val1 + val2) & 0xFFFFFFFF;
    uint32_to_bytes(sum, out);
}

// Bool Functions
void FF(const bytes X[4], const bytes Y[4], const bytes Z[4], int seq, bytes out[4]) {
    if (seq >= 0 && seq <= 15) {
        bytes tmp1[4], tmp2[4];
        xor_32bit(X, Y, tmp1);
        xor_32bit(tmp1, Z, out);
    }
    else if (seq >= 16 && seq <= 63) {
        bytes tmp1[4], tmp2[4], tmp3[4], tmp4[4];
        and_32bit(X, Y, tmp1);
        and_32bit(X, Z, tmp2);
        and_32bit(Y, Z, tmp3);
        or_32bit(tmp1, tmp2, tmp4);
        or_32bit(tmp4, tmp3, out);
    }
    else {
        cerr << "FF function error: seq out of range." << endl;
    }
}

void GG(const bytes X[4], const bytes Y[4], const bytes Z[4], int seq, bytes out[4]) {
    if (seq >= 0 && seq <= 15) {
        bytes tmp1[4], tmp2[4];
        xor_32bit(X, Y, tmp1);
        xor_32bit(tmp1, Z, out);
    }
    else if (seq >= 16 && seq <= 63) {
        bytes tmp1[4], tmp2[4], tmp3[4];
        and_32bit(X, Y, tmp1); // tmp1=X & Y
        not_32bit(X, tmp2); // tmp2=~X
        and_32bit(tmp2, Z, tmp3); // tmp3=~X & Z
        or_32bit(tmp1, tmp3, out); // out=(X & Y) | (~X & Z)
    }
    else {
        cerr << "GG function error: seq out of range." << endl;
    }
}

unsigned int constant_T(int seq) {
    if (seq >= 0 && seq <= 15) {
        return 0x79CC4519;
    }
    else if (seq >= 16 && seq <= 63) {
        return 0x7A879D8A;
    }
    else {
        cerr << "constant_T function error: seq out of range." << endl;
        return -1; // Error case
    }
}

// 算法实现函数
//   读取输入流中的用户输入（一个16进制字符串），将其填充为512的整数倍的bytes数组存储到padded_input中，并返回int型的数组长度
int Input_and_Pad(bytes* padded_input) {
    string input_hex;
    cin >> input_hex;

    // Convert hex string to bytes
    int input_len = input_hex.length() / 2;
    bytes* input_bytes = new bytes[input_len];
    for (int i = 0; i < input_len; ++i) {
        string byte_str = input_hex.substr(2 * i, 2);
        input_bytes[i] = (bytes)strtol(byte_str.c_str(), nullptr, 16);
    }

    // Calculate original bit length
    unsigned long long bit_len = input_len * 8;

    // Padding: append '1' bit (0x80), then '0' bits, then 64-bit length
    int pad_len = 0;
    int total_len = input_len + 1; // +1 for 0x80
    while ((total_len * 8) % 512 != 448) {
        ++total_len;
        ++pad_len;
    }

    // Copy input to padded_input
    for (int i = 0; i < input_len; ++i) {
        padded_input[i] = input_bytes[i];
    }
    padded_input[input_len] = 0x80; // append '1' bit
    for (int i = 0; i < pad_len; ++i) {
        padded_input[input_len + 1 + i] = 0x00;
    }

    // Append 64-bit big-endian length
    for (int i = 0; i < 8; ++i) {
        padded_input[total_len + i] = (bytes)((bit_len >> (56 - 8 * i)) & 0xFF);
    }

    delete[] input_bytes;
    return total_len + 8;
}

// 消息扩展函数
//   利用512bit的消息块生成W0~W63与W'0~W'63
static bytes W[68][4];
static bytes WW[64][4];
void extend_message(const bytes* message_block) {
    // 将64字节的message_block划分成16个4字节bytes赋值给W0~W15
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            W[i][j] = message_block[i * 4 + j];
        }
    }
    // 计算W16~W67
    bytes tmp1[4], tmp2[4], tmp3[4], tmp4[4], tmp5[4], tmp6[4];
    for (int i = 16; i < 68; ++i) {
		circular_shiftleft_32bit(W[i - 3], 15, tmp1); // tmp1 = W[i-3] << 15
		circular_shiftleft_32bit(W[i - 13], 7, tmp2); // tmp2 = W[i-13] << 7
		xor_32bit(W[i - 16], W[i - 9], tmp3); // tmp3 = W[i-16] ^ W[i-9]
		xor_32bit(tmp1, tmp3, tmp4); // tmp4 = (W[i-3] << 15) ^ W[i-16] ^ W[i-9]
		P_Permutation(tmp4, 1, tmp5); // tmp5 = P(tmp4, 1)
		xor_32bit(tmp5, tmp2, tmp6); // tmp6 = tmp5 ^ (W[i-13] << 7)
		xor_32bit(tmp6, W[i - 6], W[i]); // W[i] = tmp6 ^ W[i-16]
    }
    // 计算W'0~W'63
    for (int i = 0; i < 64; ++i) {
        xor_32bit(W[i], W[i + 4], WW[i]);
    }
    return;
}

void calculate_Vi_and_Bi(const bytes Vi[8][4], const bytes Bi[64], bytes out[8][4]) {
    // A,B,C,D,E,F,G,H分别对应Vi的8个4字节部分
    static bytes A[4], B[4], C[4], D[4], E[4], F[4], G[4], H[4];
    for (int i = 0; i < 4; ++i) {
        A[i] = Vi[0][i];
        B[i] = Vi[1][i];
        C[i] = Vi[2][i];
        D[i] = Vi[3][i];
        E[i] = Vi[4][i];
        F[i] = Vi[5][i];
        G[i] = Vi[6][i];
        H[i] = Vi[7][i];
    }
    // 中间变量SS1,SS2,TT1,TT2
    static bytes SS1[4], SS2[4], TT1[4], TT2[4];
    static bytes tmp1[4], tmp2[4], tmp3[4], tmp4[4], tmp5[4], tmp6[4], tmp7[4], tmp8[4];
    // 迭代64次
    for (int j = 0; j < 64; ++j) {
        // 计算SS1
        circular_shiftleft_32bit(A, 12, tmp1); // tmp1 = A << 12
        add_32bit(tmp1, E, tmp2); // tmp2 = (A << 12) + E
        unsigned int Ti = constant_T(j);
        uint32_to_bytes(Ti, tmp3); // 将Ti转换为4字节
        circular_shiftleft_32bit(tmp3, j % 32, tmp4); // tmp4 = Ti << (j % 32)
        add_32bit(tmp2, tmp4, tmp5); // tmp5 = ((A << 12) + E) + (Ti << (j % 32))
        circular_shiftleft_32bit(tmp5, 7, SS1); // SS1 = ((A << 12) + E + (Ti << (j % 32))) << 7
        // 计算SS2
        xor_32bit(SS1, tmp1, SS2); // SS2 = SS1 ^ (A << 12)
        // 计算TT1，现在前面的tmp都没用了，可以从tmp1开始复用
        FF(A, B, C, j, tmp1); // tmp1 = FF(A, B, C, j)
        add_32bit(tmp1, D, tmp2);
        add_32bit(tmp2, SS2, tmp3);
        add_32bit(tmp3, WW[j], TT1);
        // 计算TT2
        GG(E, F, G, j, tmp1);
        add_32bit(tmp1, H, tmp2);
        add_32bit(tmp2, SS1, tmp3);
        add_32bit(tmp3, W[j], TT2);
        //完成迭代剩余部分
        copy_32bit(C, D); // D = C
        circular_shiftleft_32bit(B, 9, C); // C = b<<<9
        copy_32bit(A, B); // B = A
        copy_32bit(TT1, A); // A = TT1
        copy_32bit(G, H); // H = G
        circular_shiftleft_32bit(F, 19, G);
        copy_32bit(E, F); // F = E
        P_Permutation(TT2, 0, E);
        // End For
        if (DEBUG_MODE) {
            printf("j=%2d: ", j);
            for (int idx = 0; idx < 4; ++idx) printf("%02X", A[idx]);
            printf(" ");
            for (int idx = 0; idx < 4; ++idx) printf("%02X", B[idx]);
            printf(" ");
            for (int idx = 0; idx < 4; ++idx) printf("%02X", C[idx]);
            printf(" ");
            for (int idx = 0; idx < 4; ++idx) printf("%02X", D[idx]);
            printf(" ");
            for (int idx = 0; idx < 4; ++idx) printf("%02X", E[idx]);
            printf(" ");
            for (int idx = 0; idx < 4; ++idx) printf("%02X", F[idx]);
            printf(" ");
            for (int idx = 0; idx < 4; ++idx) printf("%02X", G[idx]);
            printf(" ");
            for (int idx = 0; idx < 4; ++idx) printf("%02X", H[idx]);
            printf("\n");
        }
    }
    // 计算轮结果
    for (int i = 0; i < 4; ++i){
        out[0][i] = A[i];
        out[1][i] = B[i];
        out[2][i] = C[i];
        out[3][i] = D[i];
        out[4][i] = E[i];
        out[5][i] = F[i];
        out[6][i] = G[i];
        out[7][i] = H[i];
    }
}

int main() {
    // 给输入分配足够的内存
    bytes padded_input[2048] = { 0 }; // 2048 bytes should be enough
    int padded_len = Input_and_Pad(padded_input);

    // Output padded_input as hex
    if (DEBUG_MODE) {
        for (int i = 0; i < padded_len; ++i) {
            printf("%02X", padded_input[i]);
            if ((i + 1) % 32 == 0) printf("\n"); // Print 32 bytes per line for readability
        }
        if (padded_len % 32 != 0) printf("\n");
    }

    // 将padded_input转化为64字节的二维bytes数组
    int num_blocks = padded_len / 64;
    bytes** blocks = new bytes * [num_blocks];
    for (int i = 0; i < num_blocks; ++i) {
        blocks[i] = new bytes[64];
        for (int j = 0; j < 64; ++j) {
            blocks[i][j] = padded_input[i * 64 + j];
        }
    }

    // Output blocks for debugging
    if (DEBUG_MODE) {
        for (int i = 0; i < num_blocks; ++i) {
            printf("Block %d: ", i);
            for (int j = 0; j < 64; ++j) {
                printf("%02X", blocks[i][j]);
            }
            printf("\n");
        }
    }

    // SM3轮函数
    bytes(*V)[8][4] = new bytes[num_blocks + 1][8][4];
    // V[0]的值是IV向量
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 4; ++j) {
            V[0][i][j] = IV[i][j];
        }
    }
    for (int i = 0; i < num_blocks; i++) {
        // 生成第i轮的W与WW数组
        extend_message(blocks[i]);
        if (DEBUG_MODE) {
            cout << "After extending block " << i << ":\n";
            printf("W[%d]:\n", i);
            for (int k = 0; k < 68; ++k) {
                printf("W[%02d]: ", k);
                for (int l = 0; l < 4; ++l) {
                    printf("%02X", W[k][l]);
                }
                printf("\n");
            }
            printf("WW[%d]:\n", i);
            for (int k = 0; k < 64; ++k) {
                printf("WW[%02d]: ", k);
                for (int l = 0; l < 4; ++l) {
                    printf("%02X", WW[k][l]);
                }
                printf("\n");
            }
        }
        // 计算V[i+1]
        static bytes BlockOut[8][4];
        calculate_Vi_and_Bi(V[i], blocks[i], BlockOut);
        xor_256bit(BlockOut, V[i], V[i+1]);
    }

    // 输出杂凑值V[num_blocks]
    printf("SM3 Hash: ");
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%02X", V[num_blocks][i][j]);
        }
    }
    printf("\n");

    // Remember to free memory if you use blocks later
    // for (int i = 0; i < num_blocks; ++i) delete[] blocks[i];
    // delete[] blocks;

    return 0;
}