#ifndef PARTICLEDATACONTAINER_H
#define PARTICLEDATACONTAINER_H
#include <memory>
#include<iomanip>
#include <glm/vec3.hpp> // glm::vec3


class ParticleDataContainer
{
public:
    ParticleDataContainer() { }
    explicit ParticleDataContainer(size_t max_size);
    ~ParticleDataContainer() { }
    ParticleDataContainer(ParticleDataContainer &) = delete;
    ParticleDataContainer operator=(ParticleDataContainer&) = delete;

    void Generate(size_t max_size);
    void Kill(size_t id);
    void KillAll();
    void Wake(size_t id);
    void WakeUpAll();
    void SwapData(size_t id_A, size_t id_B);

    std::unique_ptr<glm::vec3[]> positions_;
    std::unique_ptr<glm::vec3[]> color_;
    std::unique_ptr<glm::vec3[]> start_color_;
    std::unique_ptr<glm::vec3[]> end_color_;
    std::unique_ptr<glm::vec3[]> velocity_;
    std::unique_ptr<glm::vec3[]> acceleration_;
    std::unique_ptr<glm::vec3[]> time_;
    std::unique_ptr<bool[]> alive_;

    size_t count_{0};
    size_t count_alive_{0};
};

#endif // PARTICLEDATACONTAINER_H
