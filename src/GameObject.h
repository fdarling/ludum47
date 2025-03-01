#pragma once

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();
    virtual int type() const = 0;
};
