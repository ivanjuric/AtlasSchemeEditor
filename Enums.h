#ifndef ENUMS_H
#define ENUMS_H

enum class OrientationEnum
{
    None = 0,
    Horizontal = 1,
    Vertical = 2
};
enum class PinTypeEnum
{
    BusPin = 0,
    Square = 1,
    Circle = 2,
    In = 3,
    Out = 4,
    InOut = 5,
    SquareIn = 6,
    SquareOut = 7,
    SquareInOut = 8
};
enum class PinSideEnum
{
    None = 0,
    Left = 1,
    Right = 2
};

#endif // ENUMS_H
