
class IFluidSimulation {
public:
    virtual ~IFluidSimulation() = default;
};

class IFluidVisualization {
public:
    virtual ~IFluidVisualization() = default;
    virtual void Step() = 0;
};