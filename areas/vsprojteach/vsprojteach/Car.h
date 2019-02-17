#pragma once

class Car
{
public:
    Car();
    ~Car();
    void DriveSouth();
    void DriveNorth();
    int GetX();

private:
    int posX;   
};
