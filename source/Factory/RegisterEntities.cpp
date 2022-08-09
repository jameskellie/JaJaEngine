#include "EntityCreator.h"

#include "Factory.h"

#include "Car.h"
#include "OldMan.h"
#include "Player.h"

void EntityCreator::RegisterEntities()
{
    Register<Player> PLAYER("PLAYER");
    Register<OldMan> OLDMAN("OLDMAN");
    Register<Car> CAR("CAR");
}