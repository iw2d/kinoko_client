#pragma once


class IGObj {
public:
    virtual void Update() = 0;
};

static_assert(sizeof(IGObj) == 0x4);