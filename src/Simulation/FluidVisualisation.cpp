#include "FluidVisualisation.h"

/**
 * Constructs new abstraction of fluid visualization.
 * This class is visualizing fluid as a box where a higher density is symbolized by darker pixel
 * @param dyeMap map of "dye" - the higher density the higher dye amount
 */
FluidVisualization::FluidVisualization(IPixelMap* pixelMap)
    : _pixelMap(pixelMap)
    , _singlePixelDyeAmount(0)
{
}

/**
 * Perform step - send density map do dye map to perform necessary calculations
 * @param densityMap map of density at whole simulation
 */
void FluidVisualization::Update(const Matrix& densityMap)
{
    for (int x = 0; x < densityMap.GetSize(); ++x) {
        for (int y = 0; y < densityMap.GetSize(); ++y) {
            _singlePixelDyeAmount = 255 - std::min(static_cast<uint32_t>(densityMap(x, y)), static_cast<uint32_t>(255));
            _pixelMap->SetPixel(
                x, y, { _singlePixelDyeAmount, _singlePixelDyeAmount, _singlePixelDyeAmount, _singlePixelDyeAmount });
        }
    }
}
