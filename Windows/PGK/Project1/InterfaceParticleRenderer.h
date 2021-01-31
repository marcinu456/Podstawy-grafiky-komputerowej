#ifndef INTERFACEPARTICLERENDERER_H
#define INTERFACEPARTICLERENDERER_H

#include "particlesystem.h"
class InterfaceParticleRenderer
{
public:
    InterfaceParticleRenderer() { }
    ~InterfaceParticleRenderer() { }

    virtual void Generate(ParticleSystem * system) = 0;
    virtual void Destroy() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
};

#endif // INTERFACEPARTICLERENDERER_H
