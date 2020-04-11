#pragma once
#include "SceneObject.h"
#include "pch.h"

#include <vector>
#include <string>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Path
{
public:

	Path();
	~Path();

	Vector3 GetNextPoint(float t_offset);
	void ResetPath();

	inline void AddNode(SceneObject* node) { m_nodes.push_back(node); };
	inline void DeleteNode(int i) { m_nodes.erase(m_nodes.begin() + i); };

	inline SceneObject* GetNode(int i) { return m_nodes[i]; };
	inline std::vector<SceneObject*> GetNodes() { return m_nodes; };

	inline bool AtPathEnd() { return m_pathEnd; };

	std::wstring m_name;

private:
	std::vector<SceneObject*> m_nodes;

	float m_tension;		// Tension of Path
	float m_alpha;			// Alpha of Path
	float m_t;				// How far along the current segment the path is
	float m_speed;
	int m_currentNode;		// Start Node for current spline segment
	bool m_pathEnd;			// Whether the end of the path has been reached

	//Vector3 GetPointAlongCurrentSegment();
};

