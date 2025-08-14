pragma circom 2.0.0;

template Sigma() {
    signal input in;
    signal output out;

    signal in2;
    signal in4;

    in2 <== in * in;
    in4 <== in2 * in2;
    out <== in4 * in;  // x^5 S-box
}

template Ark(t, C, offset) {
    signal input in[t];
    signal output out[t];

    for (var i = 0; i < t; i++) {
        out[i] <== in[i] + C[offset + i];
    }
}

template Mix3() {
    // 固定矩阵用于t=3的情况
    signal input in[3];
    signal output out[3];
    
    // 手动展开矩阵乘法 (使用硬编码矩阵值)
    out[0] <== 2*in[0] + 1*in[1] + 1*in[2];
    out[1] <== 1*in[0] + 3*in[1] + 1*in[2];
    out[2] <== 1*in[0] + 1*in[1] + 4*in[2];
}

template MixLast3() {
    // 固定矩阵用于t=3的情况
    signal input in[3];
    signal output out;
    
    // 手动计算点积 (输出状态[1])
    out <== 1*in[0] + 3*in[1] + 1*in[2];
}

template Poseidon2() {
    // 固定参数配置 (n,t,d)=(256,3,5)
    var t = 3;           // 固定状态大小
    var R_F = 8;         // 全轮数
    var R_P = 56;        // 部分轮数 (Table 1)
    var d = 5;           // S-box 指数

    // 轮常数 (示例值，实际应用需替换为完整预计算值)
    var C_total[80];
    for (var i = 0; i < 80; i++) {
        C_total[i] = i;
    }

    signal input inputs[2];  // 2个输入元素
    signal input initialState;  // 初始状态 (固定为0)
    signal output out;

    // ==== 初始化状态 ====
    component initMix = Mix3();
    initMix.in[0] <== initialState;
    initMix.in[1] <== inputs[0];
    initMix.in[2] <== inputs[1];

    // ==== 前4个全轮 (R_F/2) ====
    component full_ark1[4];
    component full_sigma1[4][3];
    component full_mix1[4];

    for (var r = 0; r < 4; r++) {
        full_ark1[r] = Ark(t, C_total, r * t);
        if (r == 0) {
            for (var i = 0; i < t; i++) {
                full_ark1[r].in[i] <== initMix.out[i];
            }
        } else {
            for (var i = 0; i < t; i++) {
                full_ark1[r].in[i] <== full_mix1[r - 1].out[i];
            }
        }

        for (var i = 0; i < t; i++) {
            full_sigma1[r][i] = Sigma();
            full_sigma1[r][i].in <== full_ark1[r].out[i];
        }

        full_mix1[r] = Mix3();
        for (var i = 0; i < t; i++) {
            full_mix1[r].in[i] <== full_sigma1[r][i].out;
        }
    }

    // ==== 56个部分轮 (R_P) ====
    component part_ark[56];
    component part_sigma[56];
    component part_mix[56];

    for (var r = 0; r < 56; r++) {
        // 创建部分轮常数数组 [c, 0, 0]
        var c_vec[3];
        c_vec[0] = C_total[24 + r];  // 24 = R_F * t
        c_vec[1] = 0;
        c_vec[2] = 0;
        
        part_ark[r] = Ark(t, c_vec, 0);
        if (r == 0) {
            for (var i = 0; i < t; i++) {
                part_ark[r].in[i] <== full_mix1[3].out[i];
            }
        } else {
            for (var i = 0; i < t; i++) {
                part_ark[r].in[i] <== part_mix[r - 1].out[i];
            }
        }

        part_sigma[r] = Sigma();
        part_sigma[r].in <== part_ark[r].out[0];  // 仅对第一个字应用S-box

        part_mix[r] = Mix3();
        part_mix[r].in[0] <== part_sigma[r].out;
        part_mix[r].in[1] <== part_ark[r].out[1];  // 其他字保持不变
        part_mix[r].in[2] <== part_ark[r].out[2];
    }

    // ==== 后4个全轮 (R_F/2) ====
    component full_ark2[4];
    component full_sigma2[4][3];
    component full_mix2[4];

    for (var r = 0; r < 4; r++) {
        full_ark2[r] = Ark(t, C_total, (4 + r) * t);  // 4 = R_F/2
        if (r == 0) {
            for (var i = 0; i < t; i++) {
                full_ark2[r].in[i] <== part_mix[55].out[i];
            }
        } else {
            for (var i = 0; i < t; i++) {
                full_ark2[r].in[i] <== full_mix2[r - 1].out[i];
            }
        }

        for (var i = 0; i < t; i++) {
            full_sigma2[r][i] = Sigma();
            full_sigma2[r][i].in <== full_ark2[r].out[i];
        }

        full_mix2[r] = Mix3();
        for (var i = 0; i < t; i++) {
            full_mix2[r].in[i] <== full_sigma2[r][i].out;
        }
    }

    // ==== 输出哈希值 (状态中的第一个速率元素) ====
    component lastMix = MixLast3();
    lastMix.in[0] <== full_mix2[3].out[0];
    lastMix.in[1] <== full_mix2[3].out[1];
    lastMix.in[2] <== full_mix2[3].out[2];
    out <== lastMix.out;
}

// 主电路 (输入2个元素)
template Main() {
    signal input preimage[2];  // 隐私输入: 哈希原象
    signal input hash;           // 公开输入: 哈希值

    component poseidon = Poseidon2();
    poseidon.initialState <== 0;
    poseidon.inputs[0] <== preimage[0];
    poseidon.inputs[1] <== preimage[1];
    
    // 验证计算哈希值等于公开输入
    hash === poseidon.out;
}

component main = Main();