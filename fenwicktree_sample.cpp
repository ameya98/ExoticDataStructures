/*
Author: Ameya Daigavane
Date: 12th June, 2018
Driver program for the Fenwick Tree header.
*/

#include <iostream>
#include <vector>
#include "fenwicktree.hpp"

int main()
{
    std::vector<int> v = {1, 3, 2, 4};
    FenwickTree ft1(v);

    std::cout << ft1.range_sum(0, 0) << "\n";
    std::cout << ft1.range_sum(1, 2) << "\n";
    std::cout << ft1.range_sum(1, 3) << "\n";

    int a[10] = {1, 4, 6, 22};
    FenwickTree ft2(a, 10);

    std::cout << ft2.range_sum(8, 9) << "\n";
    std::cout << ft2.range_sum(1, 2) << "\n";
    std::cout << ft2.range_sum(1, 3) << "\n";


}
