#include "Quadtree.h"

#include "Camera.h"
#include "Entity.h"
#include "Resources.h"

#include "SDL_render.h"

void Quadtree::Notify(std::shared_ptr<Level> level)
{
    std::list<Entity *>::iterator iterator = observers.begin();
    while (iterator != observers.end())
    {
        (*iterator)->Update(collisionA, level);
        (*iterator)->Update(collisionB, level);
        ++iterator;
    }
}

Quadtree::Quadtree(const Vector2D &min, const Vector2D &max, const int depth)
    : min  (min)
    , max  (max)
    , mid  ({(min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f})
    , depth(depth) {}

Quadtree::Quadrant Quadtree::Inside(const SDL_FRect &hitbox)
{
    // Where is the object relative to the quadtree?
    Quadrant index = Quadrant::NONE;

    bool top  = hitbox.y < mid.y && hitbox.y + hitbox.h < mid.y, // Wholly in the top half
         left = hitbox.x < mid.x && hitbox.x + hitbox.w < mid.x; // Wholly in the left half

    if      ( left &&  top) index = Quadrant::NW;
    else if (!left &&  top) index = Quadrant::NE;
    else if ( left && !top) index = Quadrant::SW;
    else if (!left && !top) index = Quadrant::SE;

    bool xOverlap = hitbox.x <= mid.x && hitbox.x + hitbox.w >= mid.x, // On a vertical line
         yOverlap = hitbox.y <= mid.y && hitbox.y + hitbox.h >= mid.y; // On a horizontal line

    if (xOverlap)
    {
        if (yOverlap) index = Quadrant::ALL; // In all four quadrants
        else if (top) index = Quadrant::NWNE; // In the nw + ne quadrants
        else          index = Quadrant::SWSE; // In the sw + se quadrants
    }
    else if (yOverlap)
    {
        if (left) index = Quadrant::NWSW; // In the nw + sw quadrants
        else      index = Quadrant::NESE; // In the ne + se quadrants
    }

    return index;
}

bool Quadtree::Match(Entity *objectA, std::shared_ptr<Entity> objectB)
{
    if (   objectA->hitbox.x == objectB->hitbox.x
        && objectA->hitbox.y == objectB->hitbox.y
        && objectA->hitbox.w == objectB->hitbox.w
        && objectA->hitbox.h == objectB->hitbox.h)
        return true;

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
    std::list<std::shared_ptr<Entity>>::iterator iterator = objects.begin();
    while (iterator != objects.end())
    {
        Quadrant index = Inside((*iterator)->hitbox);

        if      (index == Quadrant::NW) nw->Insert(*iterator);
        else if (index == Quadrant::NE) ne->Insert(*iterator);
        else if (index == Quadrant::SW) sw->Insert(*iterator);
        else if (index == Quadrant::SE) se->Insert(*iterator);
        if      (index >  Quadrant::NONE) iterator = objects.erase(iterator);
        else                            ++iterator; // Keep the objects that didn't fit into any here
    }
}

void Quadtree::Insert(std::shared_ptr<Entity> object)
{
    // Don't add the object to the tree if it's not on the screen
    if (   object->hitbox.x + object->hitbox.w < min.x
        || object->hitbox.y + object->hitbox.h < min.y
        || object->hitbox.x > max.x
        || object->hitbox.y > max.y)
        return;

    Quadrant index = Inside(object->hitbox);

    // Don't place an object in this node if it has children nodes, unless...
    if (nw != nullptr)
    {
        if      (index == Quadrant::NW) nw->Insert(object);
        else if (index == Quadrant::NE) ne->Insert(object);
        else if (index == Quadrant::SW) sw->Insert(object);
        else if (index == Quadrant::SE) se->Insert(object);
        if      (index <= Quadrant::NONE) objects.push_back(object);

        return;
    }

    objects.push_back(object); // ... It doesn't fit wholly

    if (nw == nullptr && objects.size() > MAX_OBJECTS && depth < MAX_DEPTH)
    {
        Subdivide(); // Subdivide when full
    }
}

void Quadtree::Search(Entity *object, std::list<std::shared_ptr<Entity>> &returnObjects)
{
    Quadrant index = Inside(object->hitbox);

    if (nw != nullptr)
    {
        if      (index == Quadrant::NW) nw->Search(object, returnObjects);
        else if (index == Quadrant::NE) ne->Search(object, returnObjects);
        else if (index == Quadrant::SW) sw->Search(object, returnObjects);
        else if (index == Quadrant::SE) se->Search(object, returnObjects);
        else if (index == Quadrant::ALL) // Need to search all quadrants the object is touching
        {
            nw->Search(object, returnObjects);
            ne->Search(object, returnObjects);
            sw->Search(object, returnObjects);
            se->Search(object, returnObjects);
        }
        else if (index == Quadrant::NWNE)
        {
            nw->Search(object, returnObjects);
            ne->Search(object, returnObjects);
        }
        else if (index == Quadrant::SWSE)
        {
            sw->Search(object, returnObjects);
            se->Search(object, returnObjects);
        }
        else if (index == Quadrant::NWSW)
        {
            nw->Search(object, returnObjects);
            sw->Search(object, returnObjects);
        }
        else if (index == Quadrant::NESE)
        {
            ne->Search(object, returnObjects);
            se->Search(object, returnObjects);
        }
    }

    // Adds every object in the same quadrant as the input to a return vector
    std::list<std::shared_ptr<Entity>>::iterator iterator = objects.begin();
    while (iterator != objects.end())
    {
        returnObjects.push_back(*iterator);
        ++iterator;
    }
}

void Quadtree::CheckCollisions(std::shared_ptr<Level> level)
{
    // Need to use observers list not objects list because observers is the entities, objects includes map bounds
    std::list<std::shared_ptr<Entity>> returnObjects;
    std::list<Entity *>::iterator iterator = observers.begin();
    while (iterator != observers.end())
    {
        Search(*iterator, returnObjects);

        // Only run this routine from root
        if (observers.size() > 0)
        {
            std::list<std::shared_ptr<Entity>>::iterator returnObjectsIterator = returnObjects.begin();
            while (returnObjectsIterator != returnObjects.end())
            {
                // Only notify the colliding observers // TODO: There has to be a better way to avoid checking yourself than calling Match()...
                if (SDL_HasIntersectionF(&((*iterator)->hitbox), &((*returnObjectsIterator)->hitbox)) && !Match(*iterator, *returnObjectsIterator))
                {
                    (*iterator)->Update(*returnObjectsIterator, level);
                }

                ++returnObjectsIterator;
            }
        }

        returnObjects.clear();
        ++iterator;
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

    std::list<std::shared_ptr<Entity>>::iterator iterator = objects.begin();
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