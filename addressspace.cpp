#include "addressspace.h"

AddressSpace::AddressSpace()
{
}
qlonglong AddressSpace::getMin(){
    return min.toLongLong(0,16);
}

qlonglong AddressSpace::getMax(){
    return max.toLongLong(0,16);
}
