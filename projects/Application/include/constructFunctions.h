#pragma once
#include "json.hpp"

class Primitive;
class Plane;
class Light;

//using createSurface = Surface * (*)(const nlohmann::json&);
//using createLight = Light * (*)(const nlohmann::json&);

Primitive * createSphere(const nlohmann::json &json);

Primitive * createPlane(const nlohmann::json &json);

Primitive * createOBB(const nlohmann::json &json);

Light * createLight(const nlohmann::json &json);
