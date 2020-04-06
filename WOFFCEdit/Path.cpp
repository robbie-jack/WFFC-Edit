#include "Path.h"

Path::Path()
{
	m_tension = 0.0f;
	m_alpha = 0.5f;
	m_t = 0.0f;
	m_currentSegment = 0;
}

Path::~Path()
{

}

Vector3 Path::GetNextPoint(float t_offset)
{
	m_t += t_offset;

	if (m_t > 1.0f)
	{
		float new_offset = m_t - 1.0f;

		if (m_currentSegment < m_segments.size() - 1)
			m_currentSegment++;

		m_t = new_offset;
	}

	Vector3 pos0 = Vector3(m_segments[m_currentSegment].a->posX, m_segments[m_currentSegment].a->posY, m_segments[m_currentSegment].a->posZ);
	Vector3 pos1 = Vector3(m_segments[m_currentSegment].b->posX, m_segments[m_currentSegment].b->posY, m_segments[m_currentSegment].b->posZ);
	Vector3 pos2 = Vector3(m_segments[m_currentSegment].c->posX, m_segments[m_currentSegment].c->posY, m_segments[m_currentSegment].c->posZ);
	Vector3 pos3 = Vector3(m_segments[m_currentSegment].d->posX, m_segments[m_currentSegment].d->posY, m_segments[m_currentSegment].d->posZ);

	Vector3 pos = Vector3::CatmullRom(pos0, pos1, pos2, pos3, m_t);

	return pos;
}

//Vector3 Path::GetPointAlongCurrentSegment()
//{
//	Vector3 pos0 = Vector3(m_segments[m_currentSegment].a->posX, m_segments[m_currentSegment].a->posY, m_segments[m_currentSegment].a->posZ);
//	Vector3 pos1 = Vector3(m_segments[m_currentSegment].b->posX, m_segments[m_currentSegment].b->posY, m_segments[m_currentSegment].b->posZ);
//	Vector3 pos2 = Vector3(m_segments[m_currentSegment].c->posX, m_segments[m_currentSegment].c->posY, m_segments[m_currentSegment].c->posZ);
//	Vector3 pos3 = Vector3(m_segments[m_currentSegment].d->posX, m_segments[m_currentSegment].d->posY, m_segments[m_currentSegment].d->posZ);
//
//	float t01 = pow(Vector3::Distance(pos0, pos1), m_alpha);
//	float t12 = pow(Vector3::Distance(pos1, pos2), m_alpha);
//	float t23 = pow(Vector3::Distance(pos2, pos3), m_alpha);
//
//	Vector3 m1 = (1.0f - m_tension) * 
//		(pos2 - pos1 + t12 * ((pos1 - pos0) / t01 - (pos2 - pos0) / (t01 + t12)));
//}