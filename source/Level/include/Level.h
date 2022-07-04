#ifndef LEVEL_H
#define LEVEL_H

#include "Map.h"
#include "Resources.h"

#include "Observer/Subject.h"

#include <memory>
#include <string>
#include <unordered_map>

class Quadtree;

class Level : public Observer
{
    // Observer-related members
    std::shared_ptr<Object>  collision;
    std::shared_ptr<Subject> subject;

    // All the maps that are part of this level
    std::unordered_map<std::string, std::shared_ptr<Map>> maps;
    std::string                                           currentMap;

public:
    // Observer-related functions
    void Update(std::shared_ptr<void> collision) override;
    inline void RemoveObserver() { subject->Detach(this); }
    virtual void CollisionReaction();

    Level(std::shared_ptr<Subject> subject);

    bool Load        (std::shared_ptr<Resources> resources, const std::string id, const std::string source);
    bool ParseMaps   (std::shared_ptr<Resources> resources, const std::string source);
    void Render      (std::shared_ptr<Resources> resources, std::shared_ptr<Camera> camera, const bool under = true);
    void FillQuadtree(std::shared_ptr<Quadtree>  quadtree);

    inline void ClearMap(const std::string map) { maps.erase(map); }
    inline void Clean()                         { maps.clear(); }

    // Getters
    inline std::shared_ptr<Map> GetCurrentMap() { return maps[currentMap]; }

    // Setters
    inline void SetMap(const std::string map) { currentMap = map; }
};

#endif // LEVEL_H
