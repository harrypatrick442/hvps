#pragma once
class IFPGABus
{
public:
    virtual ~IFPGABus() = default;
    virtual void setGoLive(bool value) = 0;
    virtual void setInShift(bool value) = 0;
    virtual void setOutShift(bool value) = 0;
    virtual void setToOutput(bool value) = 0;
    virtual void setInValue(bool value) = 0;
    virtual bool getOutValue() = 0;
};