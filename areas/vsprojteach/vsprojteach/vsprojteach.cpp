// vsprojteach.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "algorithm.h"
#include "Car.h"

int main()
{
    Car c;
    c.DriveNorth();
    c.DriveNorth();
    c.DriveNorth();
    c.DriveNorth();
    c.DriveNorth();
    c.DriveSouth();
    c.DriveSouth();
    c.DriveSouth();
    std::cout << "Car is at " << c.GetX() << std::endl;
    std::cout << "Enter two numberes:" << std::endl;
    int n1, n2;
    std::cin >> n1 >> n2;
    auto mx = maximum(n1, n2);
    auto mn = minimum(n1, n2);
    auto av = average(n1, n2);
    std::cout << "Maximum is " << mx << std::endl;
    std::cout << "Minimum is " << mn << std::endl;
    std::cout << "Average is " << av << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
