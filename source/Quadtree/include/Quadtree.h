#ifndef QUADTREE_H
#define QUADTREE_H

#include "Observer/Subject.h"
#include "Vector2D/Vector2D.h"

#include <list>
#include <memory>

class Camera;
class Entity;
class Resources;

// A quadtree will reduce the amount of objects we need to check collision against,
// although only somewhat. It will still check against objects far away if the object
// is on a border line, or if the object you're checking against is on a border line.
// Something like a spatial hashmap would be more efficient if performance becomes a problem.
class Quadtree : public Subject
{
    // While it doesn't make a lot of real-world sense, having "NONE" be the middleground between
    // singular and multiple quadrants allows for easier code logic via < NONE being guaranteed to
    // be more than 1 quadrant and vice versa
    enum class Quadrant
    {
        NWNE,
        NWSW,
        SWSE,
        NESE,
        ALL,
        NONE,
        NW,
        NE,
        SW,
        SE
    };

    static const int MAX_DEPTH   = 5;
    static const int MAX_OBJECTS = 10;

    Vector2D min,
             max,
             mid;

    int depth;

    std::list<std::shared_ptr<Entity>> objects;

    std::unique_ptr<Quadtree> nw,
                              ne,
                              sw,
                              se;

    std::list<Entity *>     observers;
    std::shared_ptr<Entity> collisionA;
    std::shared_ptr<Entity> collisionB;

public:
    // Observer-related functions
    inline void Attach(Entity *observer) override { observers.push_back(observer); }
    inline void Detach(Entity *observer) override { observers.remove   (observer); }
    void Notify(std::shared_ptr<Level> level) override;

    Quadtree(const Vector2D &min, const Vector2D &max, const int depth = 0);

    Quadrant Inside(const SDL_FRect &hitbox);
    bool Match(Entity *objectA, std::shared_ptr<Entity> objectB);
    void Subdivide();
    void Insert(std::shared_ptr<Entity> object);
    void Search(Entity *object, std::list<std::shared_ptr<Entity>> &returnObjects);
    void CheckCollisions(std::shared_ptr<Level> level);
    void DrawTree(std::shared_ptr<Resources> resources, std::shared_ptr<Camera> camera);
    void SetBounds(std::shared_ptr<Camera> camera);
    void Clear();
    // TODO: Delete
    // TODO: Callback for deleting + reinserting object upon coordinate change
};

#endif // QUADTREE_H