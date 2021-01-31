#include "particlegenerators.h"



void SpherePositionGenerator::generate(double dt, ParticleDataContainer *p, MultithreadOverseer *task_menager, size_t start_id, size_t end_id)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(0, 2.0f * 3.14);
    float fi0;
    float fi1;
    for(size_t i = start_id; i < end_id; i++)
    {
        fi0 = dist(gen);
        fi1 = dist(gen);
        p->positions_[i] = std::move(glm::vec3(radius_ * glm::sin(fi0) * glm::cos(fi1),
                                               radius_ * glm::sin(fi0) * glm::sin(fi1),
                                               radius_ * glm::cos(fi0)));
    }
}




void OneColorGenerator::generate(double dt, ParticleDataContainer *p, MultithreadOverseer *task_menager, size_t start_id, size_t end_id)
{
    for(size_t i = start_id; i < end_id; i++)
    {
        p->color_[i] = color_;
    }
}

void BasicVelocityGenerator::generate(double dt, ParticleDataContainer *p, MultithreadOverseer *task_menager, size_t start_id, size_t end_id)
{
    for(size_t i = start_id; i < end_id; i++)
    {
        p->velocity_[i] = velocity_;
    }
}

void BasicAccelerationGenerator::generate(double dt, ParticleDataContainer *p, MultithreadOverseer *task_menager, size_t start_id, size_t end_id)
{
    for(size_t i = start_id; i < end_id; i++)
    {
        p->acceleration_[i] = acceleration_;
    }
}
