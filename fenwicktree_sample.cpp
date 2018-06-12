/*
Author: Ameya Daigavane
Date: 12th June, 2018
Sample driver program for the Fenwick Tree header.
*/

#include <iostream>
#include <vector>
#include "fenwicktree.hpp"

int main()
{
    std::vector<int> v = {1, 3, 2, 4};
    FenwickTree<int> ft1(v);

    std::cout << ft1.range_sum(0, 0) << "\n";
    std::cout << ft1.range_sum(1, 2) << "\n";
    std::cout << ft1.range_sum(1, 3) << "\n\n";

    double a[10] = {1, 4.5, 6, 22, 11, 2, 3};

    FenwickTree<double> ft2(a, 10);

    std::cout << ft2.range_sum(1, 2) << "\n";
    std::cout << ft2.range_sum(1, 3) << "\n";
    std::cout << ft2.point_sum(0) << "\n";
    std::cout << ft2.point_sum(1) << "\n";
    std::cout << ft2.point_sum(2) << "\n";
    std::cout << ft2.point_sum(3) << "\n\n";

    ft2.range_update(0, 9, 2);
    std::cout << ft2.range_sum(1, 2) << "\n";
    std::cout << ft2.range_sum(1, 3) << "\n";
    std::cout << ft2.point_sum(0) << "\n";
    std::cout << ft2.point_sum(1) << "\n";
    std::cout << ft2.point_sum(2) << "\n";
    std::cout << ft2.point_sum(3) << "\n\n";

    ft2.point_update(0, -2);
    std::cout << ft2.range_sum(1, 2) << "\n";
    std::cout << ft2.range_sum(1, 3) << "\n";
    std::cout << ft2.point_sum(0) << "\n";
    std::cout << ft2.point_sum(1) << "\n";
    std::cout << ft2.point_sum(2) << "\n";
    std::cout << ft2.point_sum(3) << "\n";
    std::cout << ft2.prefix_sum(1) << "\n\n";


}
