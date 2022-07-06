#include "Camera.h"

Camera::Camera(const SDL_Properties &properties)
{
    viewBox  = std::make_shared<SDL_FRect>();

    viewBox->x = 0.0f;
    viewBox->y = 0.0f;
    viewBox->w = (float)properties.TARGET_WIDTH;
    viewBox->h = (float)properties.TARGET_HEIGHT;
}

void Camera::Update(const Vector2D &mapDimensions, const Vector2D &tileSize)
{
    if (target == nullptr)
        return;

    viewBox->x = target->x - viewBox->w / 2.0f;
    viewBox->y = target->y - viewBox->h / 2.0f;

    if (viewBox->x < 0.0f)
        viewBox->x = 0.0f;

    if (viewBox->y < 0.0f)
        viewBox->y = 0.0f;

    float mapWidth  = mapDimensions.x * tileSize.x,
          mapHeight = mapDimensions.y * tileSize.y;

    if (viewBox->x + viewBox->w > mapWidth)
        viewBox->x = mapWidth - viewBox->w;

    if (viewBox->y + viewBox->h > mapHeight)
        viewBox->y = mapHeight - viewBox->h;
}
