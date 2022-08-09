#ifndef ENTITYCREATOR_H
#define ENTITYCREATOR_H

#include "Sequence/Sequence.h"

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

class Entity;
class Subject;

struct TextureProperties;

class EntityCreator
{
    // Singleton related members
    static EntityCreator *instance;
    static std::mutex mutex;

    EntityCreator() {}

    std::unordered_map<std::string, std::function<std::shared_ptr<Entity>(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)>> entities;

    std::shared_ptr<Entity> FactoryMethod(const std::string id, std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties);

public:
    // Singleton related functions
    EntityCreator(EntityCreator &other)   = delete;
    void operator=(const EntityCreator &) = delete;

    static EntityCreator *GetInstance();

    // TODO: Unique pointers instead?
    void RegisterEntities();
    void RegisterEntityToMap(const std::string id, std::function<std::shared_ptr<Entity>(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)> entity);
    std::shared_ptr<Entity> ParseEntity(const char *source, std::shared_ptr<Subject> subject);
    std::vector<std::shared_ptr<Entity>> ParseEntities(const std::string source, const std::string map, std::shared_ptr<Subject> subject);
};

#endif // ENTITYCREATOR_H