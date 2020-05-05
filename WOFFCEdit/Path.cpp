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

			Vector3 a = Vector3(m_nodes.at(m_currentNode)->posX, m_nodes.at(m_currentNode)->posY, m_nodes.at(m_currentNode)->posZ);
			Vector3 b = Vector3(m_nodes.at(m_currentNode + 1)->posX, m_nodes.at(m_currentNode + 1)->posY, m_nodes.at(m_currentNode + 1)->posZ);
			Vector3 c = Vector3(m_nodes.at(m_currentNode + 2)->posX, m_nodes.at(m_currentNode + 2)->posY, m_nodes.at(m_currentNode + 2)->posZ);
			Vector3 d = Vector3(m_nodes.at(m_currentNode + 3)->posX, m_nodes.at(m_currentNode + 3)->posY, m_nodes.at(m_currentNode + 3)->posZ);

			PrecalculateCatmullRom(a, b, c, d);
		}
		else
		{
			m_pathEnd = true;
		}
	}

	return GetNextCatmullRomPoint(m_t);
	//return Vector3::CatmullRom(a, b, c, d, m_t);
}

void Path::ResetPath()
{
	m_pathEnd = false;
	m_t = 0.0f;
	m_currentNode = 0;

	Vector3 a = Vector3(m_nodes.at(m_currentNode)->posX, m_nodes.at(m_currentNode)->posY, m_nodes.at(m_currentNode)->posZ);
	Vector3 b = Vector3(m_nodes.at(m_currentNode + 1)->posX, m_nodes.at(m_currentNode + 1)->posY, m_nodes.at(m_currentNode + 1)->posZ);
	Vector3 c = Vector3(m_nodes.at(m_currentNode + 2)->posX, m_nodes.at(m_currentNode + 2)->posY, m_nodes.at(m_currentNode + 2)->posZ);
	Vector3 d = Vector3(m_nodes.at(m_currentNode + 3)->posX, m_nodes.at(m_currentNode + 3)->posY, m_nodes.at(m_currentNode + 3)->posZ);

	PrecalculateCatmullRom(a, b, c, d);
}

void Path::PrecalculateCatmullRom(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
{
	float t0 = 0.0f;
	float t1 = t0 + pow(Vector3::Distance(a, b), m_alpha);
	float t2 = t1 + pow(Vector3::Distance(b, c), m_alpha);
	float t3 = t2 + pow(Vector3::Distance(c, d), m_alpha);

	Vector3 m1 = (1.0f - m_tension) * (t2 - t1) * ((b - a) / (t1 - t0) - (c - a) / (t2 - t0) + (c - b) / (t2 - t1));
	Vector3 m2 = (1.0f - m_tension) * (t2 - t1) * ((c - b) / (t2 - t1) - (d - b) / (t3 - t1) + (d - c) / (t3 - t2));

	p0 = 2.0f * (b - c) + m1 + m2;
	p1 = -3.0f * (b - c) - m1 - m1 - m2;
	p2 = m1;
	p3 = b;
}

Vector3 Path::GetNextCatmullRomPoint(float t)
{
	return p0 * t * t * t +
		p1 * t * t +
		p2 * t +
		p3;
}