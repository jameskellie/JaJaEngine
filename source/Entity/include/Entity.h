#ifndef ENTITY_H
#define ENTITY_H

#include "Transform.h"

#include "Observer/Subject.h"
#include "TextureProperties/TextureProperties.h"

#include <memory>

class Level;
class Quadtree;
class Resources;

class Entity : public Observer
{
protected:
    enum class Direction
    {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    // Observer-related members
    std::shared_ptr<Entity>  collision;
    std::shared_ptr<Subject> subject;

    float tileWidth,
          tileHeight;

    Direction facing;

    std::unique_ptr<Transform> transform;
    std::shared_ptr<Vector2D>  origin;

public:
    // Observer-related functions
    void Update(std::shared_ptr<void> collision, std::shared_ptr<Level> level) override;
    inline void RemoveObserver() { subject->Detach(this); }
    virtual void CollisionReaction(std::shared_ptr<Level> level) = 0;

    // TODO: Getter/Setter instead of public
    SDL_FRect hitbox;
    std::string loadZone;
    Vector2D    setMapPos;

    Entity() {}
    Entity(const SDL_FRect &hitbox) : hitbox(hitbox) {}
    Entity(std::shared_ptr<Subject> subject, const TextureProperties &properties);

    static bool compareY(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2);

    virtual ~Entity() {}
    virtual void Update(std::shared_ptr<Resources> resources) = 0;
    virtual void Render(std::shared_ptr<Resources> resources) = 0;
    virtual void UpdateHitbox()                               = 0;

    // Getters
    inline std::shared_ptr<Vector2D> GetOrigin() { return origin; }

    // Setters
    void SetPosition(const Vector2D &position);
    void SetOrigin  (const Vector2D &position);
};

#endif // ENTITY_H