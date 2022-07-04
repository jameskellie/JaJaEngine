#ifndef FACTORY_H
#define FACTORY_H

#include "EntityCreator.h"

template<class T>
class Registrar
{
public:
    Registrar(const std::string id)
    {
        EntityCreator::Register(id, 
            [](std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)->std::shared_ptr<Entity>
            {
                return std::make_shared<T>(subject, states, properties);
            });
    }
};

#endif // FACTORY_H