#pragma once

class TransformComponent;

class ITransformProvider
{
public:
    virtual ~ITransformProvider() = default;

    virtual TransformComponent* GetTransform() = 0;
};
