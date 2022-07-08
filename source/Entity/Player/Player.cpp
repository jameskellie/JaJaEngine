#include "Player.h"

#include "Quadtree.h"
#include "Resources.h"

Player::Player(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)
    : Entity(subject, properties)
{
    facing    = Direction::SOUTH;
    animation = std::make_unique<Animation>(properties.id, states);
    rigidBody = std::make_unique<RigidBody>();
    Idle();
}

void Player::CollisionReaction()
{
    SDL_FRect hitbox = GenerateHitbox(); // TODO: This should probably be optimised out

    if (collision->loadZone == "")
    {
        if (movingHorizontally) transform->x = (lastPos.x + hitboxMin.x < collision->hitbox.x)
                                             ? collision->hitbox.x - hitbox.w - hitboxMin.x
                                             : collision->hitbox.x + collision->hitbox.w - hitboxMin.x;
        else                    transform->y = (lastPos.y + hitboxMax.x < collision->hitbox.y)
                                             ? collision->hitbox.y - hitbox.h - hitboxMax.x
                                             : collision->hitbox.y + collision->hitbox.h - hitboxMax.x;
    }
    else
    {
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

void Player::Update(std::shared_ptr<Resources> resources, std::shared_ptr<Quadtree> quadtree)
{
    Movement(resources);

    rigidBody->Update(resources->GetEngine()->GetDeltaTime());

    // Need this for recursion in quadtree function; cannot default initialise a non-const reference argument
    std::list<std::shared_ptr<Object>> DEFAULT_LIST;

    // X Collision
    movingHorizontally = true;
    lastPos.x = transform->x;
    transform->TranslateX(rigidBody->GetPosition().x);
    quadtree->Search(GenerateHitbox(), DEFAULT_LIST);
    
    DEFAULT_LIST.clear();

    // Y Collision
    movingHorizontally = false;
    lastPos.y = transform->y;
    transform->TranslateY(rigidBody->GetPosition().y);
    quadtree->Search(GenerateHitbox(), DEFAULT_LIST);

    SetOrigin(Vector2D(transform->x, transform->y));

    animation->Update();
}

void Player::Render(std::shared_ptr<Resources> resources)
{
    animation->Draw(resources, transform->x, transform->y, tileWidth, tileHeight);
}
