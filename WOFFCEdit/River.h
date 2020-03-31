#pragma once

#include "pch.h"

#include <vector>

using namespace DirectX;

struct TerrainPoint
{
	int i;
	int j;
	float value;

	TerrainPoint(int i_ = 0, int j_ = 0, float value_ = 0.0f)
	{
		i = i_;
		j = j_;
		value = value_;
	}
};

// Class defining one section of a river
class RiverSection
{
public:

	RiverSection(TerrainPoint point1 = TerrainPoint(), TerrainPoint point2 = TerrainPoint(), TerrainPoint point3 = TerrainPoint());
	~RiverSection();

	inline TerrainPoint GetPoint(int i) { return m_points[i]; };
	inline void SetPoint(int i, TerrainPoint point) { m_points[i] = point; };

	// Derive a point along the bezier curve
	TerrainPoint FindQuadraticPoint(float t);

private:
	TerrainPoint m_points[3]; // Points to build bezier curve from

	int GetLerpPoint(int n1, int n2, float perc);
};

// Class defining a river made up of river sections
class River
{
public:

	inline RiverSection& GetSection(int i) { return m_sections[i]; };

	inline void AddSection() 
	{
		RiverSection section; m_sections.push_back(section);
	}

private:
	bool m_active;
	std::vector<RiverSection> m_sections;
};

