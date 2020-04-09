#pragma once
#include "SceneObject.h"
#include "pch.h"

#include <vector>
#include <string>

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct PathSegment
{
	SceneObject* a;
	SceneObject* b;
	SceneObject* c;
	SceneObject* d;
};

class Path
{
public:

	Path();
	~Path();

	Vector3 GetNextPoint(float t_offset);

	void AddSegment(SceneObject* a, SceneObject* b, SceneObject* c, SceneObject* d)
	{
		PathSegment segment;
		segment.a = a;
		segment.b = b;
		segment.c = c;
		segment.d = d;
		m_segments.push_back(segment);
	}

	inline PathSegment* GetSegment(int i) { return &m_segments.at(i); };
	inline std::vector<PathSegment>* GetSegments() { return &m_segments; };

	inline bool AtPathEnd() { return m_pathEnd; };

	std::wstring m_name;

private:
	std::vector<PathSegment> m_segments;

	float m_tension;		// Tension of Path
	float m_alpha;			// Alpha of Path
	float m_t;				// How far along the current segment the path is
	int m_currentSegment;	// The current segement in the gements vector
	bool m_pathEnd;			// Whether the end of the path has been reached

	//Vector3 GetPointAlongCurrentSegment();
};

