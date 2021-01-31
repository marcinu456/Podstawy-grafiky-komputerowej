#include "particlesystem.h"


ParticleSystem::ParticleSystem(size_t max_count,size_t number_of_threads) : task_menager(number_of_threads)
{
    count_ = max_count;
    particles_.Generate(max_count);

    particles_.KillAll();
}

void ParticleSystem::Update(double dt)
{
    for(auto &emiter : emiters_)
    {
        emiter->Emit(dt, &particles_, &task_menager);
    }

    for(auto &updater : updaters_)
    {
        updater->Update(dt, &particles_, &task_menager);
    }
    task_menager.WaitForAllTasks();
}

void ParticleSystem::Reset()
{
    particles_.count_alive_ = 0;
}
