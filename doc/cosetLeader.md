# 陪集首部计算器
    cosetLeader.cpp
    demo:
```c++
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
    // std::cout << c.getParityCheckMatrix() <<"\n"<< std::endl;
    // std::cout << c.getCodeWords() <<"\n"<< std::endl;
    // std::cout << c.getCosetLeaders() <<"\n"<< std::endl;
    // c.printCosetTable();
    // std::cout << std::endl;
    std::cout << c.decode("111011") << std::endl;
    std::cout << c.decode("010000") << std::endl;
    std::cout << c.decode("010100") << std::endl;
    std::cout << c.decode("110000") << std::endl;
    return 0;
}
```
