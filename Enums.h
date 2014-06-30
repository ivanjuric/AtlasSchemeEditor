#ifndef ENUMS_H
#define ENUMS_H

#pragma once


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
enum class PopupTypeEnum
{
    Automatic = 1,
    OnDemand = 2
};
enum class RuleCheckTypeEnum
{
    Error = 1,
    Warning = 2
};
enum class RuleCheckConditionEnum
{
    DivisibleByPowerOf2 = 1,
    GreaterOrEqual = 2,
    LessOrEqual = 3,
    StartsInAddressSpace = 4,
    EndsInAddressSpace = 5,
    UniqueInAddressSpace = 6,
    DivisibleBy = 7,
    InRange = 8
};
enum class CheckConnectionTypeEnum
{
    CheckOnly = 1,
    ConnectOnDemand = 2
};

#endif // ENUMS_H
