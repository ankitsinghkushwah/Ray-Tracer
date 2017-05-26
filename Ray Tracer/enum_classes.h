#ifndef INC_ENUM_CLASSES_H
#define INC_ENUM_CLASSES_H

#include<cstdint>

enum class ShapeID : std::uint8_t { SPHERE, PLANE };
enum class Direction : std::uint8_t { LEFT, RIGHT, UP, DOWN, FORWARD, BACKWARD };
enum class Material : std::uint8_t{ DIFFUSE, REFLECTIVE };

#endif // !INC_ENUM_CLASSES_H