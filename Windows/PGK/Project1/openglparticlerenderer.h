#ifndef OPENGLPARTICLERENDERER_H
#define OPENGLPARTICLERENDERER_H

#include <epoxy/gl.h>

#include "InterfaceParticleRenderer.h"
#include "particlesystem.h"
class OpenGLParticleRenderer : public InterfaceParticleRenderer
{
public:
    ParticleSystem * system_{nullptr};

    unsigned int position_buffer_{0};
    unsigned int color_buffer_{0};
    unsigned int vao_{0};
public:
    OpenGLParticleRenderer() { }
    virtual ~OpenGLParticleRenderer() { Destroy(); }

    void Generate(ParticleSystem *system) override;
    void Destroy() override;
    void Update() override;
    void Render() override;

};

#endif // OPENGLPARTICLERENDERER_H
