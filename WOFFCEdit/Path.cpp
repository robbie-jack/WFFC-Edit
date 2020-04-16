#include "Path.h"

Path::Path()
{
	m_tension = 0.0f;
	m_alpha = 0.5f;
	m_t = 0.0f;
	m_speed = 0.2f;
	m_currentNode = 0;
	m_pathEnd = false;
	m_name = L"";
}

Path::~Path()
{

}

Vector3 Path::GetNextPoint(float t_offset)
{
	m_t += t_offset * m_speed;

	if (m_t > 1.0f)
	{
		float new_offset = m_t - 1.0f;

		if (m_currentNode < m_nodes.size() - 4)
		{
			m_currentNode++;
			m_t = new_offset;
		}
		else
		{
			m_pathEnd = true;
		}
	}

	Vector3 a = Vector3(m_nodes.at(m_currentNode)->posX, m_nodes.at(m_currentNode)->posY, m_nodes.at(m_currentNode)->posZ);
	Vector3 b = Vector3(m_nodes.at(m_currentNode + 1)->posX, m_nodes.at(m_currentNode + 1)->posY, m_nodes.at(m_currentNode + 1)->posZ);
	Vector3 c = Vector3(m_nodes.at(m_currentNode + 2)->posX, m_nodes.at(m_currentNode + 2)->posY, m_nodes.at(m_currentNode + 2)->posZ);
	Vector3 d = Vector3(m_nodes.at(m_currentNode + 3)->posX, m_nodes.at(m_currentNode + 3)->posY, m_nodes.at(m_currentNode + 3)->posZ);

	return Vector3::CatmullRom(a, b, c, d, m_t);
}

void Path::ResetPath()
{
	m_pathEnd = false;
	m_t = 0.0f;
	m_currentNode = 0;
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