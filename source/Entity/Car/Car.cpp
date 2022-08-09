#include "Car.h"

#include "Resources.h"

Car::Car(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)
    : Entity(subject, properties)
{
    facing    = Direction::WEST;
    animation = std::make_unique<Animation>(properties.id, states);
    rigidBody = std::make_unique<RigidBody>();
    animation->SetState("drive_west");
    rigidBody->ApplyForceX(-64.0f);
}

Car::~Car()
{
    RemoveObserver();
}

void Car::CollisionReaction(std::shared_ptr<Level> level)
{
    (void)level;
}

void Car::Update(std::shared_ptr<Resources> resources)
{
    movingHorizontally = !movingHorizontally;

    rigidBody->Update(resources->GetEngine()->GetDeltaTime());
    // rigidBody->RemoveForce();

    lastPos.x = transform->x;
    lastPos.y = transform->y;

    transform->Translate(rigidBody->GetPosition());

    if (transform->x < -50.0f)
    {
        transform->x = 550.0f;
    }

    animation->Update();
    UpdateHitbox();
}

void Car::Render(std::shared_ptr<Resources> resources)
{
    animation->Draw(resources, transform->x, transform->y, tileWidth, tileHeight);
}

void Car::UpdateHitbox()
{
    hitbox.x = transform->x + hitboxMin.x;
    hitbox.y = transform->y + hitboxMin.y;
    hitbox.w = hitboxMax.x;
    hitbox.h = hitboxMax.y;
}