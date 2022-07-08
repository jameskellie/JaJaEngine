#ifndef ENTITYREGISTER_H
#define ENTITYREGISTER_H

#include "Factory.h"

#include "Player.h"

void RegisterEntity(const char *type)
{
    std::string temp = std::string(type);

    if (temp == "PLAYER") static Registrar<Player> registrar("PLAYER");
}

#endif // ENTITYREGISTER_H