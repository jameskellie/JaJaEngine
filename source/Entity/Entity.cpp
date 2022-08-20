#include "Entity.h"

bool Entity::compareY(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2)
{
    return (entity1->hitbox.y < entity2->hitbox.y);
}

Entity::Entity()
{
    origin = std::make_shared<Vector2D>();
}

Entity::Entity(std::shared_ptr<Subject> subject, const TextureProperties &properties)
    : subject   (subject)
    , tileWidth (properties.tileWidth)
    , tileHeight(properties.tileHeight)
{
    this->subject->Attach(this);

    transform = std::make_unique<Transform>(properties.x, properties.y);
    origin    = std::make_shared<Vector2D>();
}

void Entity::Update(std::shared_ptr<void> collision, std::shared_ptr<Level> level)
{
    this->collision = std::static_pointer_cast<Entity>(collision);

    CollisionReaction(level);
}

void Entity::SetPosition(const Vector2D &position)
{
    transform->x = position.x;
    transform->y = position.y;
}

void Entity::SetOrigin(const bool flag)
{
    if (flag)
    {
        origin->x = hitbox.x + hitbox.w / 2.0f;
        origin->y = hitbox.y + hitbox.h / 2.0f;
    }
    else
    {
        origin->x = lastPos.x + tileWidth  / 2.0f;
        origin->y = lastPos.y + tileHeight / 2.0f;
    }
}