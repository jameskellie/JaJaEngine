#ifndef ENTITY_H
#define ENTITY_H

#include "Transform.h"

#include "Object/Object.h"
#include "Observer/Subject.h"
#include "TextureProperties/TextureProperties.h"

#include <memory>

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
    std::shared_ptr<Object>  collision;
    std::shared_ptr<Subject> subject;

    float tileWidth,
          tileHeight;

    Direction facing;

    std::unique_ptr<Transform> transform;
    std::shared_ptr<Vector2D>  origin;

public:
    // Observer-related functions
    void Update(std::shared_ptr<void> collision) override;
    inline void RemoveObserver() { subject->Detach(this); }
    virtual void CollisionReaction() = 0;

    Entity(std::shared_ptr<Subject> subject, const TextureProperties &properties);

    virtual ~Entity() {}
    virtual void      Update(std::shared_ptr<Resources> resources, std::shared_ptr<Quadtree> quadtree) = 0;
    virtual void      Render(std::shared_ptr<Resources> resources)                                     = 0;
    virtual SDL_FRect GenerateHitbox()                                                                 = 0;

    // Getters
    inline std::shared_ptr<Vector2D> GetOrigin() { return origin; }

    // Setters
    void SetOrigin(const float x, const float y);
};

#endif // ENTITY_H