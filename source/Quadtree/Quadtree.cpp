#include "Quadtree.h"

#include "Camera.h"
#include "Resources.h"

#include "SDL_render.h"

#include <iostream>

void Quadtree::Notify()
{
    std::list<Observer *>::iterator iterator = observers.begin();
    while (iterator != observers.end())
    {
        (*iterator)->Update(collision);
        ++iterator;
    }
}

Quadtree::Quadtree(const Vector2D &min, const Vector2D &max, const int depth)
    : min  (min)
    , max  (max)
    , mid  ({(min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f})
    , depth(depth) {}

int Quadtree::Inside(std::shared_ptr<Object> object)
{
    // Where is the object relative to the quadtree?
    int index = -1;

    bool top  = object->hitbox.y < mid.y && object->hitbox.y + object->hitbox.h < mid.y, // Wholly in the top half
         left = object->hitbox.x < mid.x && object->hitbox.x + object->hitbox.w < mid.x; // Wholly in the left half

    if      ( left &&  top) index = 0;
    else if (!left &&  top) index = 1;
    else if ( left && !top) index = 2;
    else if (!left && !top) index = 3;

    bool xOverlap = object->hitbox.x <= mid.x && object->hitbox.x + object->hitbox.w >= mid.x, // On a vertical line
         yOverlap = object->hitbox.y <= mid.y && object->hitbox.y + object->hitbox.h >= mid.y; // On a horizontal line

    if (xOverlap)
    {
        if (yOverlap) index = -2; // In all four quadrants
        else if (top) index = -3; // In the nw + ne quadrants
        else          index = -4; // In the sw + se quadrants
    }
    else if (yOverlap)
    {
        if (left) index = -5; // In the nw + sw quadrants
        else      index = -6; // In the ne + se quadrants
    }

    return index;
}

// TODO: This function is wasting resources if we assume all objects are unique by default
// Furthermore, it should check if the object is inside an existing object, not just equal to it
bool Quadtree::Match(std::shared_ptr<Object> object)
{
    std::list<std::shared_ptr<Object>>::iterator iterator = objects.begin();
    while (iterator != objects.end())
    {
        if (   (*iterator)->hitbox.x == object->hitbox.x
            && (*iterator)->hitbox.y == object->hitbox.y
            && (*iterator)->hitbox.w == object->hitbox.w
            && (*iterator)->hitbox.h == object->hitbox.h)
            return true;

        ++iterator;
    }

    return false;
}

void Quadtree::Subdivide()
{
    // Create the four children quadrants
    nw = std::make_unique<Quadtree>(Vector2D({min.x, min.y}), Vector2D({mid.x, mid.y}), depth + 1);
    ne = std::make_unique<Quadtree>(Vector2D({mid.x, min.y}), Vector2D({max.x, mid.y}), depth + 1);
    sw = std::make_unique<Quadtree>(Vector2D({min.x, mid.y}), Vector2D({mid.x, max.y}), depth + 1);
    se = std::make_unique<Quadtree>(Vector2D({mid.x, mid.y}), Vector2D({max.x, max.y}), depth + 1);

    // Check which child quadrant every object in the current node should move into, then place it there
    std::list<std::shared_ptr<Object>>::iterator iterator = objects.begin();
    while (iterator != objects.end())
    {
        int index = Inside((*iterator));

        if      (index == 0) nw->Insert(*iterator);
        else if (index == 1) ne->Insert(*iterator);
        else if (index == 2) sw->Insert(*iterator);
        else if (index == 3) se->Insert(*iterator);
        if      (index > -1) iterator = objects.erase(iterator);
        else                 ++iterator; // Keep the objects that didn't fit into any here
    }
}

void Quadtree::Insert(std::shared_ptr<Object> object)
{
    // if (Match(object)) return; // TODO: See Match() function comment

    // Don't add the object to the tree if it's not on the screen
    if (   object->hitbox.x + object->hitbox.w < min.x
        || object->hitbox.y + object->hitbox.h < min.y
        || object->hitbox.x > max.x
        || object->hitbox.y > max.y)
        return;

    int index = Inside(object);

    // Don't place an object in this node if it has children nodes, unless...
    if (nw != nullptr)
    {
        if      (index == 0) nw->Insert(object);
        else if (index == 1) ne->Insert(object);
        else if (index == 2) sw->Insert(object);
        else if (index == 3) se->Insert(object);
        if      (index <  0) objects.push_back(object);

        return;
    }

    objects.push_back(object); // ... It doesn't fit wholly

    if (nw == nullptr && objects.size() > MAX_OBJECTS && depth < MAX_DEPTH)
    {
        Subdivide(); // Subdivide when full
    }
}

void Quadtree::Search(const SDL_FRect &object, std::list<std::shared_ptr<Object>> &returnObjects)
{
    int index = Inside(std::make_unique<Object>(object));

    if (nw != nullptr)
    {
        if      (index ==  0) nw->Search(object, returnObjects);
        else if (index ==  1) ne->Search(object, returnObjects);
        else if (index ==  2) sw->Search(object, returnObjects);
        else if (index ==  3) se->Search(object, returnObjects);
        else if (index == -2) // Need to search all quadrants the object is touching
        {
            nw->Search(object, returnObjects);
            ne->Search(object, returnObjects);
            sw->Search(object, returnObjects);
            se->Search(object, returnObjects);
        }
        else if (index == -3)
        {
            nw->Search(object, returnObjects);
            ne->Search(object, returnObjects);
        }
        else if (index == -4)
        {
            sw->Search(object, returnObjects);
            se->Search(object, returnObjects);
        }
        else if (index == -5)
        {
            nw->Search(object, returnObjects);
            sw->Search(object, returnObjects);
        }
        else if (index == -6)
        {
            ne->Search(object, returnObjects);
            se->Search(object, returnObjects);
        }
    }

    std::list<std::shared_ptr<Object>>::iterator iterator = objects.begin();
    while (iterator != objects.end())
    {
        returnObjects.push_back(*iterator);
        ++iterator;
    }

    if (observers.size() > 0)
    {
        iterator = returnObjects.begin();
        while (iterator != returnObjects.end())
        {
            if (SDL_HasIntersectionF(&(object), &((*iterator)->hitbox)))
            {
                collision = *iterator;
                Notify();
            }

            ++iterator;
        }

        // DEBUG: Uncomment to print nearby objects to console
        // iterator = returnObjects.begin();
        // std::cout << "nearby: "<<  std::endl;
        // while (iterator != returnObjects.end())
        // {
        //     std::cout << (*iterator)->hitbox.x << " " << (*iterator)->hitbox.y << " " << (*iterator)->hitbox.w << " " << (*iterator)->hitbox.h << std::endl;
        //     ++iterator;
        // }
        // std::cout << std::endl;
    }
}

void Quadtree::DrawTree(std::shared_ptr<Resources> resources, std::shared_ptr<Camera> camera)
{
    if (nw != nullptr)
    {
        SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 255, 255, 255, 200);
        SDL_RenderDrawLineF(resources->GetEngine()->GetRenderer(), min.x - camera->viewBox->x, mid.y - camera->viewBox->y, max.x - camera->viewBox->x, mid.y - camera->viewBox->y);
        SDL_RenderDrawLineF(resources->GetEngine()->GetRenderer(), mid.x - camera->viewBox->x, min.y - camera->viewBox->y, mid.x - camera->viewBox->x, max.y - camera->viewBox->y);

        nw->DrawTree(resources, camera);
        ne->DrawTree(resources, camera);
        sw->DrawTree(resources, camera);
        se->DrawTree(resources, camera);
    }

    std::list<std::shared_ptr<Object>>::iterator iterator = objects.begin();
    while (iterator != objects.end())
    {
        if ((*iterator)->hitbox.x != -1)
        {
            SDL_FRect rect = {(*iterator)->hitbox.x - camera->viewBox->x, (*iterator)->hitbox.y - camera->viewBox->y, (*iterator)->hitbox.w, (*iterator)->hitbox.h};

            if      (depth == 0) SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 255, 0  , 0  , 127);
            else if (depth == 1) SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 0  , 0  , 255, 127);
            else if (depth == 2) SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 0  , 255, 0  , 127);
            else if (depth == 3) SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 0  , 255, 255, 127);
            else if (depth == 4) SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 255, 255, 0  , 127);
            else                 SDL_SetRenderDrawColor(resources->GetEngine()->GetRenderer(), 255, 0  , 255, 127);

            SDL_RenderFillRectF(resources->GetEngine()->GetRenderer(), &rect);
        }

        ++iterator;
    }
}

void Quadtree::SetBounds(std::shared_ptr<Camera> camera)
{
    this->min = Vector2D({camera->viewBox->x, camera->viewBox->y});
    this->max = Vector2D({camera->viewBox->x + camera->viewBox->w, camera->viewBox->y + camera->viewBox->h});
    this->mid = Vector2D({(min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f});
}

void Quadtree::Clear()
{
    objects.clear();

    nw.reset(nullptr);
    ne.reset(nullptr);
    sw.reset(nullptr);
    se.reset(nullptr);
}