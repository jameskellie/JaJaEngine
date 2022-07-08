#include "Entity.h"

Entity::Entity(std::shared_ptr<Subject> subject, const TextureProperties &properties)
    : subject   (subject)
    , tileWidth (properties.tileWidth)
    , tileHeight(properties.tileHeight)
{
    this->subject->Attach(this);

    transform = std::make_unique<Transform>(properties.x, properties.y);
    origin    = std::make_unique<Vector2D>();

    SetOrigin(Vector2D(properties.x, properties.y));
}

void Entity::Update(std::shared_ptr<void> collision)
{
    this->collision = std::static_pointer_cast<Object>(collision);

    CollisionReaction();
}

void Entity::SetPosition(const Vector2D &position)
{
    transform->x = position.x;
    transform->y = position.y;
}

void Entity::SetOrigin(const Vector2D &position)
{
    origin->x = position.x + tileWidth  / 2.0f;
    origin->y = position.y + tileHeight / 2.0f;
}