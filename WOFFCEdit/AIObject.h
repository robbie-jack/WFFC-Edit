#pragma once

#include "SceneObject.h"
#include "Path.h"

class AIObject : public SceneObject
{
public:

	AIObject();
	~AIObject();

	Path path;
};