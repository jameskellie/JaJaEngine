#ifndef ENTITYREGISTER_H
#define ENTITYREGISTER_H

#include "Factory.h"

#include "Player.h"
#include "OldMan.h"

void RegisterEntity(const char *type)
{
    std::string temp = std::string(type);

    if (temp == "PLAYER") static Registrar<Player> registrar("PLAYER");
    if (temp == "OLDMAN") static Registrar<OldMan> registrar("OLDMAN");
}

#endif // ENTITYREGISTER_H