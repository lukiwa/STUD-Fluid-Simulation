#include "DyeMap.h"

/**
 * Constructs new dye map based on pixel map
 * @param pixelMap map of pixels
 */
DyeMap::DyeMap(PixelMap& pixelMap)
    : _pixelMap(pixelMap)
    , _singlePixelDyeAmount(0)
{
    _pixelMap.Clear();
}

/**
 * Sets dye (how dark a pixel is) based on density.
 * Performs squishing values to 0-255 (the higher density the darker (closer to 0) pixel is
 *
 * @param x x coordinate
 * @param y y coordinate
 * @param amount amount of dye that should be set
 */
void DyeMap::SetDye(int x, int y, double amount)
{
    _singlePixelDyeAmount = 255 - std::min(static_cast<uint32_t>(amount), static_cast<uint32_t>(255));
    _pixelMap.SetPixel(
        x, y, { _singlePixelDyeAmount, _singlePixelDyeAmount, _singlePixelDyeAmount, _singlePixelDyeAmount });
}
