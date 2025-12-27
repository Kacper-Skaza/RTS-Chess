#pragma once

class View
{
protected:

public:
    View() {};
    virtual ~View() = default;
    virtual void render() = 0;
};