#ifndef PARTICLEUPDATER_H
#define PARTICLEUPDATER_H
#include "particledatacontainer.h"
#include "multithreadoverseer.h"
class ParticleUpdater
{
public:
    ParticleUpdater() { }
    virtual ~ParticleUpdater() {}
    virtual void Update(double dt, ParticleDataContainer *p, MultithreadOverseer *task_menager) = 0;
};

#endif // PARTICLEUPDATER_H
