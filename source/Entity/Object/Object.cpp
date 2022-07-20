#include "Object.h"

#include "Quadtree.h"
#include "Resources.h"

void Object::CollisionReaction(std::shared_ptr<Level> level)
{
    (void)level;
}

void Object::Update(std::shared_ptr<Resources> resources)
{
    (void)resources;
}

void Object::Render(std::shared_ptr<Resources> resources)
{
    (void)resources;
}

void Object::UpdateHitbox()
{

}