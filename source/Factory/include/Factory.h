#ifndef FACTORY_H
#define FACTORY_H

#include "EntityCreator.h"

template<class T>
class Register
{
public:
    Register(const std::string id)
    {
        EntityCreator::GetInstance()->RegisterEntityToMap(id, 
            [](std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)->std::shared_ptr<Entity>
            {
                return std::make_shared<T>(subject, states, properties);
            });
    }
};

#endif // FACTORY_H