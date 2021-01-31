#include "particleupdaters.h"

void LawOfUniversalGravitationUpdater::Update(double dt, ParticleDataContainer *p, MultithreadOverseer *task_menager)
{
    size_t range_per_task = p->count_alive_ / task_menager->number_of_workers_;
    size_t begin_index = 0;
    for(size_t i = 0; i < task_menager->number_of_workers_ - 1; i++)
    {
        task_menager->AddTask(std::async(std::launch::deferred, &LawOfUniversalGravitationUpdater::Calculate, this, static_cast<float>(dt), p, begin_index, begin_index + range_per_task));
        begin_index += range_per_task;
    }
    task_menager->AddTask(std::async(std::launch::deferred, &LawOfUniversalGravitationUpdater::Calculate, this, static_cast<float>(dt), p, begin_index, p->count_alive_));

}

void LawOfUniversalGravitationUpdater::Calculate(float dt, ParticleDataContainer *p, size_t start_id, size_t end_id)
{
    glm::vec3 particleCenterVector;
    for(size_t i = start_id; i < end_id; i++)
    {
        particleCenterVector =    center_position_ - p->positions_[i];
        //std::cout << "wektor "<< particleCenterVector.x << " " << particleCenterVector.y << particleCenterVector.z << std::endl;
        //std::cout << "pozycja "<< p->positions_[i].x << " " << p->positions_[i].y << p->positions_[i].z << std::endl;
        glm::vec3 abc = center_mass_ * particleCenterVector / glm::pow(glm::length(particleCenterVector), 3.0f) * dt ;
        p->velocity_[i] += center_mass_ * particleCenterVector / glm::pow(glm::length(particleCenterVector), 3.0f) * dt ;
        p->positions_[i] += p->velocity_[i] * dt;
    }
}
