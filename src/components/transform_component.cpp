#include "components/transform_component.h"

TransformComponent::TransformComponent(float x, float y, float rotation, float scaleX, float scaleY) {
    position = Vector2{x, y};
    this->rotation = rotation;
    scale = Vector2{scaleX, scaleY};
}