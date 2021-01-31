#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H
#include <cstdlib>

#include "multithreadoverseer.h"
#include"particledatacontainer.h"
class ParticleGenerator
{
public:
    ParticleGenerator() { }
    virtual ~ParticleGenerator() { }
    virtual void generate(double dt, ParticleDataContainer *p, MultithreadOverseer *task_menager, size_t start_id, size_t end_id) = 0;
};

#endif // PARTICLEGENERATOR_H

