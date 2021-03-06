#include "River.h"

RiverSection::RiverSection(TerrainPoint point1, TerrainPoint point2, TerrainPoint point3)
{
	m_points[0] = point1;
	m_points[1] = point2;
	m_points[2] = point3;
}

RiverSection::~RiverSection()
{

}

// Derive point(t) of bezier curve using linear interpolation
TerrainPoint RiverSection::FindQuadraticPoint(float t)
{
	// Get Lerp of lines between points 0/1 and 1/2
	int ia = Lerp(m_points[0].i, m_points[1].i, t);
	int ja = Lerp(m_points[0].j, m_points[1].j, t);
	int va = Lerp(m_points[0].value, m_points[1].value, t);
	int ib = Lerp(m_points[1].i, m_points[2].i, t);
	int jb = Lerp(m_points[1].j, m_points[2].j, t);
	int vb = Lerp(m_points[1].value, m_points[2].value, t);

	// Get Lerp of line between points 0-1 and 1-2
	int i = Lerp(ia, ib, t);
	int j = Lerp(ja, jb, t);
	int value = Lerp(va, vb, t);

	return TerrainPoint(i, j, value);
}

// Linear Interpolation Between Two Values
int RiverSection::Lerp(int n1, int n2, float perc)
{
	int diff = n2 - n1;

	return n1 + (diff * perc);
}