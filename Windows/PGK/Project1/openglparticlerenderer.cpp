#include "openglparticlerenderer.h"


void OpenGLParticleRenderer::Generate(ParticleSystem *system)
{
    assert(system != nullptr);

    system_ = system;

    const size_t count = system_->NumberOfParticles();
    for(int i = 0; i < count; i++)
    {
    }
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &position_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * count, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glGenBuffers(1, &color_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * count, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindVertexArray(0);
}

void OpenGLParticleRenderer::Destroy()
{
    if (position_buffer_ != 0)
    {
        glDeleteBuffers(1, &position_buffer_);
        position_buffer_ = 0;
    }

    if (color_buffer_ != 0)
    {
        glDeleteBuffers(1, &color_buffer_);
        color_buffer_ = 0;
    }
}

void OpenGLParticleRenderer::Update()
{
    assert(system_ != nullptr);
    assert(position_buffer_ > 0 && color_buffer_ > 0);

    const size_t count = system_->NumberOfParticles();
    if(count > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, count*sizeof(float)* 3, system_->FinalData()->positions_.get());

        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, count*sizeof(float)* 3, system_->FinalData()->color_.get());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void OpenGLParticleRenderer::Render()
{
    glBindVertexArray(vao_);

    const size_t count = system_->NumberOfAliveParticles();
    if (count > 0)
    {
        glDrawArrays(GL_POINTS, 0, count);
    }
    glBindVertexArray(0);
}


