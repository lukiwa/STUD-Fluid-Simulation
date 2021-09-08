#pragma once
#include "../OpenGL/PixelMap.h"

class PixelMap;
class IDyeMap {
public:
    virtual ~IDyeMap() = default;
    virtual void SetDye(int x, int y, double amount) = 0;
};

class DyeMap : public IDyeMap {
public:
    DyeMap(PixelMap& pixelMap);
    ~DyeMap() = default;
    DyeMap(const DyeMap&) = delete;
    DyeMap& operator=(const DyeMap&) = delete;

    void SetDye(int x, int y, double amount) override;

private:
    PixelMap& _pixelMap;
    uint8_t _singlePixelDyeAmount;
};
