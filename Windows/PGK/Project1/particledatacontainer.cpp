#include "particledatacontainer.h"



ParticleDataContainer::ParticleDataContainer(size_t max_size)
{
    Generate(max_size);
}

void ParticleDataContainer::Generate(size_t max_size)
{
    count_ = max_size;
    count_alive_ = 0;

    positions_.reset(new glm::vec3[max_size]);
    color_.reset(new glm::vec3[max_size]);
    start_color_.reset(new glm::vec3[max_size]);
    end_color_.reset(new glm::vec3[max_size]);
    velocity_.reset(new glm::vec3[max_size]);
    acceleration_.reset(new glm::vec3[max_size]);
    time_.reset(new glm::vec3[max_size]);
    alive_.reset(new bool[max_size]);

}

void ParticleDataContainer::Kill(size_t id)
{
    alive_[id] = false;
    count_alive_--;
    SwapData(id, count_alive_);
}

void ParticleDataContainer::KillAll()
{
    for(size_t i = 0; i < count_; i++)
    {
            alive_[i] = false;
    }
    count_alive_= 0;
}

void ParticleDataContainer::Wake(size_t id)
{
    alive_[id] = true;
    SwapData(id, count_alive_);
    count_alive_++;
}

void ParticleDataContainer::WakeUpAll()
{
    for(size_t i = 0; i < count_; i++)
    {
            alive_[i] = true;
    }
    count_alive_= count_;
}

void ParticleDataContainer::SwapData(size_t id_A, size_t id_B)
{
    std::swap(positions_[id_A], positions_[id_B]);
    std::swap(color_[id_A], color_[id_B]);
    std::swap(start_color_[id_A], end_color_[id_B]);
    std::swap(velocity_[id_A], velocity_[id_B]);
    std::swap(acceleration_[id_A], acceleration_[id_B]);
    std::swap(time_[id_A], time_[id_B]);
    std::swap(alive_[id_A], alive_[id_B]);
}
