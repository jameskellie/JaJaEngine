#include "Player.h"

#include "Level.h"
#include "Resources.h"

Player::Player(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)
    : Entity(subject, properties)
{
    loadZone  = "PLAYER";
    facing    = Direction::SOUTH;
    animation = std::make_unique<Animation>(properties.id, states);
    rigidBody = std::make_unique<RigidBody>();
    Idle();
}

void Player::CollisionReaction(std::shared_ptr<Level> level)
{
    if (collision->loadZone == "")
    {
        if (movingHorizontally) transform->x = (lastPos.x + hitboxMin.x <= collision->hitbox.x)
                                             ? collision->hitbox.x - (hitboxMin.x + hitboxMax.x)
                                             : (collision->hitbox.x + collision->hitbox.w) - hitboxMin.x;
        else                    transform->y = (lastPos.y + hitboxMin.y <= collision->hitbox.y)
                                             ? collision->hitbox.y - (hitboxMin.y + hitboxMax.y)
                                             : (collision->hitbox.y + collision->hitbox.h) - hitboxMin.y;
    }
    else
    {
        level->SetMap(collision->loadZone);
        SetPosition(collision->setMapPos);
    }
}

void Player::Idle()
{
    switch (facing)
    {
        case Direction::NORTH:
        animation->SetState("idle_north");
        break;
        
        case Direction::SOUTH:
        animation->SetState("idle_south");
        break;

        case Direction::EAST:
        animation->SetState("idle_east");
        break;

        case Direction::WEST:
        animation->SetState("idle_west");
        break;
    }

    rigidBody->RemoveForce();
}

void Player::Movement(std::shared_ptr<Resources> resources)
{
    Idle();

    float verticalForce   = resources->GetInputHandler()->GetMovementDirection(MovementDirection::VERTICAL),
          horizontalForce = resources->GetInputHandler()->GetMovementDirection(MovementDirection::HORIZONTAL);

    // Move Upward
    if (verticalForce < 0.0f)
    {
        facing = Direction::NORTH;
        animation->SetState("walk_north");
    }
    // Move Downward
    else if (verticalForce > 0.0f)
    {
        facing = Direction::SOUTH;
        animation->SetState("walk_south");
    }
    // Move Left
    if (horizontalForce < 0.0f)
    {
        facing = Direction::WEST;
        animation->SetState("walk_west");
    }
    // Move Right
    else if (horizontalForce > 0.0f)
    {
        facing = Direction::EAST;
        animation->SetState("walk_east");
    }

    rigidBody->ApplyForceX(runSpeed * horizontalForce);
    rigidBody->ApplyForceY(runSpeed * verticalForce);
}

void Player::Update(std::shared_ptr<Resources> resources)
{
    movingHorizontally = !movingHorizontally;
    Movement(resources);
    rigidBody->Update(resources->GetEngine()->GetDeltaTime());

    // X Collision
    if (movingHorizontally)
    {
        lastPos.x = transform->x;
        transform->TranslateX(rigidBody->GetPosition().x);
    }
    // Y Collision
    else
    {
        lastPos.y = transform->y;
        transform->TranslateY(rigidBody->GetPosition().y);
    }

    SetOrigin(Vector2D(lastPos.x, lastPos.y));
    animation->Update();
    UpdateHitbox();
}

void Player::Render(std::shared_ptr<Resources> resources)
{
    animation->Draw(resources, transform->x, transform->y, tileWidth, tileHeight);
}

void Player::UpdateHitbox()
{
    hitbox.x = transform->x + hitboxMin.x;
    hitbox.y = transform->y + hitboxMin.y;
    hitbox.w = hitboxMax.x;
    hitbox.h = hitboxMax.y;
}