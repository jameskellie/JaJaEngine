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

Player::~Player()
{
    RemoveObserver();
}

void Player::CollisionReaction(std::shared_ptr<Level> level)
{
    if (collision->loadZone == "")
    {
        // Colliding with left edge
        if (lastPos.x + hitboxMin.x + hitboxMax.x <= collision->hitbox.x && lastPos.y + hitboxMin.y + hitboxMax.y >= collision->hitbox.y && lastPos.y + hitboxMin.y <= collision->hitbox.y + collision->hitbox.h)
            transform->x = collision->hitbox.x - (hitboxMin.x + hitboxMax.x);
        // Colliding with right edge
        if (lastPos.x + hitboxMin.x >= collision->hitbox.x + collision->hitbox.w && lastPos.y + hitboxMin.y + hitboxMax.y >= collision->hitbox.y && lastPos.y + hitboxMin.y <= collision->hitbox.y + collision->hitbox.h)
            transform->x = (collision->hitbox.x + collision->hitbox.w) - hitboxMin.x;
        // Colliding with top edge
        if (lastPos.y + hitboxMin.y + hitboxMax.y <= collision->hitbox.y && lastPos.x + hitboxMin.x + hitboxMax.x >= collision->hitbox.x && lastPos.x + hitboxMin.x <= collision->hitbox.x + collision->hitbox.w)
            transform->y = collision->hitbox.y - (hitboxMin.y + hitboxMax.y);
        // Colliding with bottom edge
        if (lastPos.y + hitboxMin.y >= collision->hitbox.y + collision->hitbox.h && lastPos.x + hitboxMin.x + hitboxMax.x >= collision->hitbox.x && lastPos.x + hitboxMin.x <= collision->hitbox.x + collision->hitbox.w)
            transform->y = (collision->hitbox.y + collision->hitbox.h) - hitboxMin.y;
    }
    else if (collision->loadZone == "OLDMAN")
    {
        Vector2D force = rigidBody->GetForce();

        // Colliding with left edge
        if (lastPos.x + hitboxMin.x + hitboxMax.x <= collision->hitbox.x && lastPos.y + hitboxMin.y + hitboxMax.y >= collision->hitbox.y && lastPos.y + hitboxMin.y <= collision->hitbox.y + collision->hitbox.h)
        {
            collision->rigidBody->ApplyForceX(force.x);
            transform->x = collision->hitbox.x - (hitboxMin.x + hitboxMax.x);
        }
        // Colliding with right edge
        if (lastPos.x + hitboxMin.x >= collision->hitbox.x + collision->hitbox.w && lastPos.y + hitboxMin.y + hitboxMax.y >= collision->hitbox.y && lastPos.y + hitboxMin.y <= collision->hitbox.y + collision->hitbox.h)
        {
            collision->rigidBody->ApplyForceX(force.x);
            transform->x = (collision->hitbox.x + collision->hitbox.w) - hitboxMin.x;
        }
        // Colliding with top edge
        if (lastPos.y + hitboxMin.y + hitboxMax.y <= collision->hitbox.y && lastPos.x + hitboxMin.x + hitboxMax.x >= collision->hitbox.x && lastPos.x + hitboxMin.x <= collision->hitbox.x + collision->hitbox.w)
        {
            collision->rigidBody->ApplyForceY(force.y);
            transform->y = collision->hitbox.y - (hitboxMin.y + hitboxMax.y);
        }
        // Colliding with bottom edge
        if (lastPos.y + hitboxMin.y >= collision->hitbox.y + collision->hitbox.h && lastPos.x + hitboxMin.x + hitboxMax.x >= collision->hitbox.x && lastPos.x + hitboxMin.x <= collision->hitbox.x + collision->hitbox.w)
        {
            collision->rigidBody->ApplyForceY(force.y);
            transform->y = (collision->hitbox.y + collision->hitbox.h) - hitboxMin.y;
        }
    }
    else if (collision->loadZone == "CAR")
    {
        Vector2D force = rigidBody->GetForce();

        // Colliding with left edge
        if (lastPos.x + hitboxMin.x + hitboxMax.x <= collision->hitbox.x && lastPos.y + hitboxMin.y + hitboxMax.y >= collision->hitbox.y && lastPos.y + hitboxMin.y <= collision->hitbox.y + collision->hitbox.h)
        {
            transform->x = collision->hitbox.x - (hitboxMin.x + hitboxMax.x);
        }
        // Colliding with right edge
        else if (lastPos.x + hitboxMin.x >= collision->hitbox.x + collision->hitbox.w && lastPos.y + hitboxMin.y + hitboxMax.y >= collision->hitbox.y && lastPos.y + hitboxMin.y <= collision->hitbox.y + collision->hitbox.h)
        {
            transform->x = (collision->hitbox.x + collision->hitbox.w) - hitboxMin.x;
        }
        // Colliding with top edge
        else if (lastPos.y + hitboxMin.y + hitboxMax.y <= collision->hitbox.y && lastPos.x + hitboxMin.x + hitboxMax.x >= collision->hitbox.x && lastPos.x + hitboxMin.x <= collision->hitbox.x + collision->hitbox.w)
        {
            transform->y = collision->hitbox.y - (hitboxMin.y + hitboxMax.y);
        }
        // Colliding with bottom edge
        else if (lastPos.y + hitboxMin.y >= collision->hitbox.y + collision->hitbox.h && lastPos.x + hitboxMin.x + hitboxMax.x >= collision->hitbox.x && lastPos.x + hitboxMin.x <= collision->hitbox.x + collision->hitbox.w)
        {
            transform->y = (collision->hitbox.y + collision->hitbox.h) - hitboxMin.y;
        }
        else
        {
            switch(collision->facing)
            {
                case Direction::NORTH:
                rigidBody->ApplyForceY(-130.0f + abs(force.x));
                break;

                case Direction::SOUTH:
                rigidBody->ApplyForceY(130.0f + abs(force.x));
                break;

                case Direction::EAST:
                rigidBody->ApplyForceX(130.0f + abs(force.x));
                break;

                case Direction::WEST:
                rigidBody->ApplyForceX(-130.0f + abs(force.x));
                break;
            }
        }
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
}

void Player::Movement(std::shared_ptr<Resources> resources)
{
    Idle();

    float verticalForce   = resources->GetInputHandler()->GetMovementDirection(MovementDirection::VERTICAL),
          horizontalForce = resources->GetInputHandler()->GetMovementDirection(MovementDirection::HORIZONTAL);
    
    Vector2D force = rigidBody->GetForce();

    if (horizontalForce == 0.0f || abs(force.x) > runSpeed) rigidBody->ReduceForceX(2);
    if (verticalForce   == 0.0f || abs(force.y) > runSpeed) rigidBody->ReduceForceY(2);

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

    if (abs(force.x) < runSpeed) rigidBody->ApplyForceX(2 * horizontalForce);
    if (abs(force.y) < runSpeed) rigidBody->ApplyForceY(2 * verticalForce);
}

void Player::Update(std::shared_ptr<Resources> resources)
{
    Movement(resources);
    rigidBody->Update(resources->GetEngine()->GetDeltaTime());

    lastPos.x = transform->x;
    lastPos.y = transform->y;

    transform->Translate(rigidBody->GetPosition());

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