#include "DyeMap.h"

DyeMap::DyeMap(PixelMap& pixelMap)
    : _pixelMap(pixelMap)
{
    _pixelMap.Clear();
}

void DyeMap::AddDye(int x, int y, uint8_t amount)
{
    _pixelMap.SetPixel(x, y, { amount, amount, amount, amount });
}
