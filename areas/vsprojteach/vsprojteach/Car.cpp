#include "Car.h"

Car::Car()
{
    posX = 0;
}

Car::~Car()
{
}

void Car::DriveNorth()
{
    posX++;
}

int Car::GetX()
{
    return posX;
}

void Car::DriveSouth()
{
    posX--;
}
