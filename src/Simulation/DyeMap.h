#pragma once
#include "../OpenGL/PixelMap.h"

class PixelMap;
class IDyeMap {
public:
    virtual ~IDyeMap() = default;
    virtual void AddDye(int x, int y, uint8_t amount) = 0;
};

class DyeMap : public IDyeMap {
public:
    DyeMap(PixelMap& pixelMap);
    ~DyeMap() = default;
    DyeMap(const DyeMap&) = delete;
    DyeMap& operator=(const DyeMap&) = delete;

    void AddDye(int x, int y, uint8_t amount) override;

private:
    PixelMap& _pixelMap;
};
