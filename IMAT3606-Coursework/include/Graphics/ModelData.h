#pragma once
#ifndef MODELDATA_H
#define MODELDATA_H
#include "Material.h"
#include <vector>
using std::vector;

class ModelData
{
public:
	Material material;
	unsigned int vaoHandle;
	vector<unsigned int> vboHandles;
	size_t indexSize;
};

#endif // !MODELDATA_H

