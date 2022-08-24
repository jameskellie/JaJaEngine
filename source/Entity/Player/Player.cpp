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

    horizontalSlide    = false;
    verticalSlide      = false;
}

Player::~Player()
{
    RemoveObserver();
}

void Player::CollisionReaction(std::shared_ptr<Level> level)
{
    bool left   = false,
         right  = false,
         top    = false,
         bottom = false;

    Vector2D force = rigidBody->GetForce();
    std::shared_ptr<Vector2D> collisionOrigin = collision->GetOrigin();
    std::shared_ptr<Vector2D> origin          = GetOrigin();

    // X-Axis collision
    if (lastPos.y + hitboxMin.y + hitboxMax.y > collision->lastHitbox.y && lastPos.y + hitboxMin.y < collision->lastHitbox.y + collision->hitbox.h)
    {
        if (origin->x < collisionOrigin->x)
            left = true;
        else
            right = true;
    }
    // Y-Axis collision
    if (lastPos.x + hitboxMin.x + hitboxMax.x > collision->lastHitbox.x && lastPos.x + hitboxMin.x < collision->lastHitbox.x + collision->hitbox.w)
    {
        if (origin->y < collisionOrigin->y)
            top = true;
        else
            bottom = true;
    }

    // Diagonal collisions
    if (lastPos.y + hitboxMin.y + hitboxMax.y < collision->lastHitbox.y && lastPos.x + hitboxMin.x + hitboxMax.x < collision->lastHitbox.x)
    {
        top  = true;
        left = true;
    }
    if (lastPos.y + hitboxMin.y + hitboxMax.y < collision->lastHitbox.y && lastPos.x + hitboxMin.x > collision->lastHitbox.x + collision->hitbox.w)
    {
        top   = true;
        right = true;
    }
    if (lastPos.y + hitboxMin.y > collision->lastHitbox.y + collision->hitbox.h && lastPos.x + hitboxMin.x + hitboxMax.x < collision->lastHitbox.x)
    {
        bottom = true;
        left   = true;
    }
    if (lastPos.y + hitboxMin.y > collision->lastHitbox.y + collision->hitbox.h && lastPos.x + hitboxMin.x > collision->lastHitbox.x + collision->hitbox.w)
    {
        bottom = true;
        right  = true;
    }

    // Edge case
    if (!top && !bottom && !left && !right)
    {
        transform->x = lastPos.x;
        transform->y = lastPos.y;
    }

    if (left)
    {
        if (collision->loadZone != "CAR")
        {
            transform->x = collision->hitbox.x - (hitboxMin.x + hitboxMax.x);
            if (horizontalSlide) rigidBody->ApplyForceX(-force.x);
        }
        if (collision->loadZone == "OLDMAN")
        {
            collision->rigidBody->ApplyForceX(force.x);
            collision->horizontalSlide = true;
        }
    }
    if (right)
    {
        if (collision->loadZone != "CAR")
        {
            transform->x = (collision->hitbox.x + collision->hitbox.w) - hitboxMin.x;
            if (horizontalSlide) rigidBody->ApplyForceX(-force.x);
        }
        if (collision->loadZone == "OLDMAN")
        {
            collision->rigidBody->ApplyForceX(force.x);
            collision->horizontalSlide = true;
        }
    }
    if (top)
    {
        if (collision->loadZone != "CAR")
        {
            transform->y = collision->hitbox.y - (hitboxMin.y + hitboxMax.y);
            if (verticalSlide) rigidBody->ApplyForceY(-force.y);
        }
        if (collision->loadZone == "OLDMAN")
        {
            collision->rigidBody->ApplyForceY(force.y);
            collision->verticalSlide = true;
        }
    }
    if (bottom)
    {
        if (collision->loadZone != "CAR")
        {
            transform->y = (collision->hitbox.y + collision->hitbox.h) - hitboxMin.y;
            if (verticalSlide) rigidBody->ApplyForceY(-force.y);
        }
        if (collision->loadZone == "OLDMAN")
        {
            collision->rigidBody->ApplyForceY(force.y);
            collision->verticalSlide = true;
        }
    }

    if (collision->loadZone == "CAR")
    {
        if (collision->facing == Direction::NORTH && top)
        {
            rigidBody->ApplyForceY(-200.0f);
            verticalSlide = true;
        }
        else if (collision->facing == Direction::SOUTH && bottom)
        {
            rigidBody->ApplyForceY(-200.0f);
            verticalSlide = true;
        }
        else if (collision->facing == Direction::EAST && right)
        {
            rigidBody->ApplyForceX(-200.0f);
            horizontalSlide = true;
        }
        else if (collision->facing == Direction::WEST && left)
        {
            rigidBody->ApplyForceX(-200.0f);
            horizontalSlide = true;
        }
        else
        {
            transform->x = lastPos.x;
            transform->y = lastPos.y;
        }
    }

    if (collision->loadZone != "CAR" && collision->loadZone != "OLDMAN" && collision->loadZone != "")
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

    Vector2D force = rigidBody->GetForce();

    if (horizontalSlide)
    {
        rigidBody->ReduceForceX(runSpeed * resources->GetEngine()->GetDeltaTime());

        if (force.x == 0.0f)
            horizontalSlide = false;
    }
    else
    {
        if (abs(force.x) <= runSpeed)
            rigidBody->RemoveForceX();

        rigidBody->ApplyForceX(runSpeed * horizontalForce);
    }

    if (verticalSlide)
    {
        rigidBody->ReduceForceY(runSpeed * resources->GetEngine()->GetDeltaTime());

        if (force.y == 0.0f)
            verticalSlide = false;
    }
    else
    {
        if (abs(force.y) <= runSpeed)
            rigidBody->RemoveForceY();
    
        rigidBody->ApplyForceY(runSpeed * verticalForce);
    }
}

void Player::Update(std::shared_ptr<Resources> resources)
{
    Movement(resources);

    rigidBody->Update(resources->GetEngine()->GetDeltaTime());

    lastPos.x  = transform->x;
    lastPos.y  = transform->y;
    lastHitbox = hitbox;

    transform->Translate(rigidBody->GetPosition());

    SetOrigin();
    animation->Update();
    UpdateHitbox();
}

void Player::Render(std::shared_ptr<Resources> resources)
{
    animation->Draw(resources, lastPos.x, lastPos.y, tileWidth, tileHeight);
}

void Player::UpdateHitbox()
{
    hitbox.x = transform->x + hitboxMin.x;
    hitbox.y = transform->y + hitboxMin.y;
    hitbox.w = hitboxMax.x;
    hitbox.h = hitboxMax.y;
}