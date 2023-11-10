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
#include<assert.h>
#include<concepts>
#include<bitset>
#include<array>
#include<vector>
#include<iostream>
#include<stdexcept>
#include<string>
#include<algorithm>
#include<map>
/**
 * @brief 调试信息
 * @author SJ
 */
#define _DINFO "[ " __FILE__ ":" + std::to_string(__LINE__) + "]"
template<typename T>
concept Addable = requires(T a, T b) {
    a+b;
};
template<typename T>
concept MultiplyAble = requires(T a, T b) {
    a*b;
};
/**
 * @brief Z2上的元素，*为且运算，+为异或运算
 *
 */
class Z2 {
    unsigned char _val;
 public:
    explicit(false) Z2(unsigned char val):_val(val) {
        if (val > 1) {
            throw std::invalid_argument(_DINFO +
            "val must be 0 or 1 but val = " + std::to_string(val));
        }
    }
    Z2():Z2(0) {}
    ~Z2() = default;
    unsigned char getVal() const {
        return _val;
    }
    friend Z2 operator+(const Z2& a, const Z2& b) {
        return Z2(a._val ^ b._val);
    }
    friend Z2 operator*(const Z2& a, const Z2& b) {
        return Z2(a._val & b._val);
    }
    friend std::ostream& operator<<(std::ostream& os, const Z2& h) {
        os << static_cast<int>(h.getVal());
        return os;
    }
};
template<typename T>
requires Addable<T> && MultiplyAble<T>
class Matrix {
 private:
    using _Matrix = std::vector<std::vector<T>>;
    _Matrix data;
    size_t M;
    size_t N;

 public:
    explicit Matrix(_Matrix data):data(data) {
        M = data.size();
        N = data[0].size();
    }
    explicit Matrix(size_t m, size_t n) :
        Matrix(_Matrix(m, std::vector<T>(n))) {}
    Matrix(std::initializer_list<std::initializer_list<T>> list) :
        Matrix(list.size(), list.begin()->size()) {
        int i = 0;
        int j = 0;
        // copy
        for (auto& l : list) {
            j = 0;
            for (auto val : l) {
                data[i][j] = val;
                j++;
            }
            i++;
        }
        if (i != M || j != N) {
            throw std::invalid_argument("invalid shape of initializer_list for Matrix");
        }
    }
    /// may it's not good
    // Matrix():Matrix(_Matrix()) {}
    ~Matrix() = default;
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (size_t i = 0; i < m.getRowNum(); ++i) {
            os << "[";
            for (size_t j = 0; j < m.getColNum(); ++j) {
                os << m[i][j] << " ]"[j + 1 ==m.getColNum()];
            }
            // i think we don't need a default \n in end
            if (i + 1 != m.getRowNum())
                os << std::endl;
        }
        return os;
    }
    friend Matrix operator+(const Matrix&a, const Matrix& b) {
        if (a.getRowNum()!= b.getRowNum() || a.getColNum()!= b.getColNum()) {
            throw std::logic_error(_DINFO + "Matrix addition error, "
            "a.rowNum!= b.rowNum or a.colNum!= b.colNum"
            "a: " + std::to_string(a.getRowNum())+
            ","+std::to_string(a.getColNum()) +
            " b: " + std::to_string(b.getRowNum())+
            "," + std::to_string(b.getColNum()));
        }
        Matrix res(a.getRowNum(), a.getColNum());
        for (size_t i = 0; i < a.getRowNum(); ++i) {
            for (size_t j = 0; j < a.getColNum(); ++j) {
                res[i][j] = a[i][j] + b[i][j];
            }
        }
        return res;
    }
    friend Matrix operator*(const Matrix& a, const Matrix& b) {
        if (a.getColNum() != b.getRowNum()) {
            throw std::logic_error(_DINFO "Matrix multiplication error, "
            "a.colNum != b.rowNum"
            "a.colNum = " + std::to_string(a.getColNum())+
            "b.rowNum = " + std::to_string(b.getRowNum()));
        }
        Matrix res(a.getRowNum(), b.getColNum());
        for (size_t i = 0; i < a.getRowNum(); ++i) {
            for (size_t j = 0; j < b.getColNum(); ++j) {
                for (size_t k = 0; k < a.getColNum(); ++k) {
                    res[i][j] = res[i][j] + a[i][k] * b[k][j];
                }
            }
        }
        return res;
    }
    friend Matrix operator*(T k, Matrix& m) {
        Matrix res = m;
        for (size_t i = 0; i < m.getRowNum(); ++i) {
            for (size_t j = 0; j < m.getColNum(); ++j) {
                res[i][j] *= k;
            }
        }
        return res;
    }
    friend Matrix operator*(Matrix& m, T k) {
        return k*m;
    }
    size_t getRowNum() const {
        return M;
    }
    size_t getColNum() const {
        return N;
    }
    std::vector<T>& operator[](size_t i) {
        if (i >= M) {
            throw std::out_of_range(_DINFO "index out of range in Matrix,"
            "index = " + std::to_string(i) + " size = " + std::to_string(M));
        }
        return data[i];
    }
    std::vector<T> operator[](size_t i)const {
        if (i >= M) {
            throw std::out_of_range(_DINFO "index out of range in Matrix,"
            "index = " + std::to_string(i) + " size = " + std::to_string(M));
        }
        return data[i];
    }
    /**
     * @brief 返回 m*n 的单位阵
     * a[i][i] = T(1), other = 0
     * @author SJ
     * @param m 行数
     * @param n 列数
     * @return Matrix
     */
    static inline Matrix IdentifyMatrix(size_t m) {
        // todo: 莫些域上的单位矩阵可能不满足以下形式， 待完善
        Matrix res(m, m);
        for (size_t i = 0; i < m; ++i) {
            res[i][i] = T(1);  // 单位元
        }
        return res;
    }
};

/**
 * @brief CosetLeader
 * e_H: B^M -> B^N
 * @author SJ
 */
class CosetLeader {
 public:
    using Mat2 = Matrix<Z2>;

 private:
    const size_t M;
    const size_t N;
    const size_t R;
    Mat2 H;
    Mat2 codeWords;
    std::vector<Mat2> cosetTable;
    Mat2 cosetLeaders;

 private:
    /**
     * @brief 用于辅助生成特定权的word的全排列
     * 借助index参数来区分
     */
    class _B{
     public:
        size_t index;
        Z2 val;
        _B(size_t i, Z2 v) : index(i), val(v) {}
        _B() : _B(0, 0) {};
        ~_B() = default;
        friend bool operator<(const _B& l, const _B& r) {
            return l.index < r.index;
        }
    };

 public:
    /**
     * @brief Construct a new Coset Leader object
     * @author SJ
     * @param h 奇偶校验矩阵上半部分, H = (h I)'
     */
    explicit(false) CosetLeader(Mat2 h) :
    M(h.getRowNum()), N(h.getRowNum()+h.getColNum()),
    R(h.getColNum()), H(Mat2(N, M)),
    codeWords(Mat2(1 << M, N)),
    cosetTable(std::vector<Mat2>(1 << R, Mat2(1 << M, N))),
    cosetLeaders(Mat2(1 << R, N)) {
        // if (M > N) {
        //     throw std::invalid_argument(_DINFO + "m should < n");
        // }
        /// generate H
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                if (i < M) {
                    // h
                    H[i][j] = h[i][j];
                } else {
                    // I
                    i-M == j ? H[i][j] = Z2(1) : H[i][j] = Z2(0);
                }
            }
        }

        generateCodeWards();

        int rowTail = 0;
        std::map<size_t, bool> vis;

        auto generateCoset = [&](const std::vector<_B>& leader) {
            for (int i = 0; i < N; i++)
                cosetLeaders[rowTail][i] = leader[i].val;
            for (int i = 0; i < (1 << M); i++) {
                auto cw = Mat2({codeWords[i]});
                std::vector<Z2> vec;
                for (const auto& v : leader)vec.push_back(v.val);
                auto led = Mat2({vec});
                vis[word2num((led + cw)[0])] = 1;
                cosetTable[rowTail][i] = cw[0];
            }
            rowTail++;
        };

        for (int cnt = 0; cnt <= N && rowTail != (1 << R); ++cnt) {  // 枚举1的个数
            std::vector<_B> leader(N);
            for (int i = 0; i < N; ++i) {
                leader[i].index = i;
                if (i < cnt)leader[i].val = 0;
            }
            do {
                if (!vis.count(word2num(leader))) {
                    generateCoset(leader);
                }
            } while (std::next_permutation(leader.begin(), leader.end()));
        }
    }
    // CosetLeader(std::initializer_list<std::initializer_list<Z2>> list) :
    // CosetLeader(Mat2(list)) {}
    ~CosetLeader() = default;
    Mat2 getParityCheckMatrix() const {
        return H;
    }
    Mat2 getCodeWords() const {
        return codeWords;
    }
    std::vector<Mat2> getCosetTable() const {
        return cosetTable;
    }
    Mat2 getCosetLeaders() const {
        return cosetLeaders;
    }
    Mat2 encode(const Mat2& word) {
        if (M != word.getColNum() || 1 != word.getRowNum()) {
            throw std::invalid_argument(_DINFO "error shape of word");
        }
        Mat2 res(1, N);
        for (int i = 0; i < M;  i++)
            res[0][i] = word[0][i];
        for (int i = M; i < N; i++) {
            res[0][i] = 0;
            for (int j = 0; j < R; j++) {
                res[0][i] = res[0][i] + word[0][j]*H[j][i-M];
            }
        }
        return res;
    }
    Mat2 decode(const Mat2& codeword) {
        if (N != codeword.getColNum() || 1 != codeword.getRowNum()) {
            throw std::invalid_argument(_DINFO "error shape of codeword");
        }
        Mat2 res(1, M);
        size_t syndrome = word2num((codeword*H)[0]);
        for (size_t i = 0; i < cosetLeaders.getRowNum(); i++) {
            Mat2 led({cosetLeaders[i]});
            if (syndrome == word2num((led*H)[0])) {
                led = led + codeword;  // epsilon xor x_t
                for (size_t j = 0; j < M; j++)
                    res[0][j] = led[0][j];
                return res;
            }
        }
    }
    // 将num转化为2进制的1*length的矩阵
    inline static Mat2 num2word(unsigned int num, const size_t length) {
        Mat2 res(1, length);
        for (int i = 0; i < length && num; ++i) {
            res[0][length-i-1] = num&1;
            num >>= 1;
        }
        return res;
    }

 private:
    inline static size_t word2num(std::vector<Z2> arr) {
        size_t res = 0ull;
        for (const auto& v : arr) {
            res <<= 1;
            if (v.getVal()) res |= 1;
        }
        return res;
    }
    inline static size_t word2num(std::vector<_B> arr) {
        std::vector<Z2> tmp;
        for (const auto& v : arr) {
            tmp.push_back(v.val);
        }
        return word2num(tmp);
    }
    void generateCodeWards() {
        for (size_t i = 0ull; i < (1 << M); i++) {
            auto m = CosetLeader::num2word(i, M);
            codeWords[i] = encode(m)[0];
        }
    }
};
int main() {
    // auto m = Matrix<int>::IdentifyMatrix(5);
    // Matrix<int> m(3, 5);
    // std::cout << m << std::endl;
    // auto v = Mat2({
    // {1, 0, 0},
    // {0, 1, 0},
    // {0, 0, 1}});
    auto c = CosetLeader({
    {1, 0, 0},
    {1, 1, 0},
    {0, 1, 1}});
    std::cout << c.getParityCheckMatrix() <<"\n"<< std::endl;
    std::cout << c.getCodeWords() <<"\n"<< std::endl;
    std::cout << c.getCosetLeaders() <<"\n"<< std::endl;
    for (const auto& v : c.getCosetTable()) {
        for (int i = 0; i < v.getRowNum(); i++) {
            std::cout << CosetLeader::Mat2({v[i]}) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
