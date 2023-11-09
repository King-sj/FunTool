/**
 * @copyright ==================================================================
 *  Copyright (c) 2023-11-09.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the
 *  distribution.
 *  3. All advertising materials mentioning features or use of this software
 *  must display the following acknowledgement:
 *  This product includes software developed by the SJ Group. and
 *  its contributors.
 *  4. Neither the name of the Group nor the names of its contributors may
 *  be used to endorse or promote products derived from this software
 *  without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY SongJian, GROUP AND CONTRIBUTORS
 *  ===================================================================
 * @file cosetLeader.cpp
 * @author KSJ
 * @date 2023-11-09
 * @version 0.1.0
 * @brief 陪集首部计算器
 * @todo 挖坑， 1.完成自动计算陪集首部
 * 2.完成根据奇偶校验矩阵初始化
 * 3.自动encode-decode
*/
#include<bitset>
#include<array>
/**
 * @brief CosetLeader
 * e_H: B^M -> B^N
 * @author SJ
 * @tparam M
 * @tparam N
 */
template <size_t M, size_t N>
class CosetLeader {
    constexpr size_t R = N-M;
    using Line = std::array<std::bitset<N>, (1 << M)>;
    using Table = std::array<Line, (1 << R)>;
 private:
    Table data;
 public:

    explicit CosetLeader(Line line) {
        data[0] = line;
        for (size_t i = 1; i < (1 << (N-M)); ++i) {
            data[i] = data[i-1];
            for (size_t j = 0; j < M; ++j) {
                if (data[i][j]) {
                    data[i] ^= data[i-1];
                }
            }
        }
    }
};
int main() {
    return 0;
}
