#include<iostream>
#include<vector>
using namespace std;
int main() {
    std::vector<std::vector<unsigned char>> v{
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}};
    for (auto i : v) {
        for (auto j : i) cout << j << " ";
        cout << endl;
    }
    return 0;
}