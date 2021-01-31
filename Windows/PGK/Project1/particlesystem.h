#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <memory>
#include <vector>

#include "multithreadoverseer.h"
#include "particledatacontainer.h"
#include "particleemitter.h"
#include "particleupdater.h"

class ParticleSystem
{
protected:
    MultithreadOverseer task_menager;

    size_t count_;

    std::vector<std::shared_ptr<ParticleEmitter>> emiters_;
    std::vector<std::shared_ptr<ParticleUpdater>> updaters_;

public:
    ParticleDataContainer particles_;
    explicit ParticleSystem(size_t max_count, size_t number_of_threads);
    virtual ~ParticleSystem() { }

    ParticleSystem(const ParticleSystem &) = delete;
    ParticleSystem &operator=(const ParticleSystem &) = delete;

    virtual void Update(double dt);
    virtual void Reset();

    virtual size_t NumberOfParticles() const { return particles_.count_; }
    virtual size_t NumberOfAliveParticles() const { return particles_.count_; }

    void AddEmiter(std::shared_ptr<ParticleEmitter> emiter) { emiters_.push_back(emiter); }
    void AddUpdater(std::shared_ptr<ParticleUpdater> updater) { updaters_.push_back(updater); }

    ParticleDataContainer *FinalData() { return &particles_; }

    static size_t ComputeMemoryUsage(const ParticleSystem &p);
};

#endif // PARTICLESYSTEM_H
