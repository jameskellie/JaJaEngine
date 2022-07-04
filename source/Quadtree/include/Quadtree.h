#ifndef QUADTREE_H
#define QUADTREE_H

#include "Object/Object.h"
#include "Observer/Subject.h"
#include "Vector2D/Vector2D.h"

#include <list>
#include <memory>

class Camera;
class Resources;

// A quadtree will reduce the amount of objects we need to check collision against,
// although only somewhat. It will still check against objects far away if the object
// is on a border line, or if the object you're checking against is on a border line.
// Something like a spatial hashmap would be more efficient if performance becomes a problem.
class Quadtree : public Subject
{
    static const int MAX_DEPTH   = 5;
    static const int MAX_OBJECTS = 10;

    Vector2D min,
             max,
             mid;

    int depth;

    std::list<std::shared_ptr<Object>> objects;

    std::unique_ptr<Quadtree> nw,
                              ne,
                              sw,
                              se;

    std::list<Observer *>   observers;
    std::shared_ptr<Object> collision;

public:
    // Observer-related functions
    inline void Attach(Observer *observer) override { observers.push_back(observer); }
    inline void Detach(Observer *observer) override { observers.remove   (observer); }
    void Notify() override;

    Quadtree(const Vector2D &min, const Vector2D &max, const int depth = 0);

    int Inside(std::shared_ptr<Object> object);
    bool Match(std::shared_ptr<Object> object);
    void Subdivide();
    void Insert(std::shared_ptr<Object> object);
    void Search(const SDL_FRect &object, std::list<std::shared_ptr<Object>> &returnObjects);
    void DrawTree(std::shared_ptr<Resources> resources, std::shared_ptr<Camera> camera);
    void SetBounds(std::shared_ptr<Camera> camera);
    void Clear();
    // TODO: Delete
    // TODO: Callback for deleting + reinserting object upon coordinate change
};

#endif // QUADTREE_H