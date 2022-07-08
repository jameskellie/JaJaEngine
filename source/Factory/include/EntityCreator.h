#ifndef ENTITYCREATOR_H
#define ENTITYCREATOR_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class Entity;
class Subject;

struct Sequence;
struct TextureProperties;

class EntityCreator
{
    static std::unordered_map<std::string, std::function<std::shared_ptr<Entity>(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)>> entities;

    static std::shared_ptr<Entity> ParseEntity(const char *source, std::shared_ptr<Subject> subject);
    static std::shared_ptr<Entity> FactoryMethod(const std::string id, std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties);

public:
    // TODO: Unique pointers instead?
    static void Register(const std::string id, std::function<std::shared_ptr<Entity>(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)> entity);
    static std::vector<std::shared_ptr<Entity>> ParseEntities(const std::string source, std::shared_ptr<Subject> subject);
    
    static inline void Clean() { entities.clear(); }
};

#endif // ENTITYCREATOR_H