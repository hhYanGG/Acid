#include "JointData.hpp"

namespace acid
{
	JointData::JointData(const uint32_t &index, const std::string &nameId, const Matrix4 &bindLocalTransform) :
		m_index(index),
		m_nameId(nameId),
		m_bindLocalTransform(bindLocalTransform),
		m_children(std::vector<JointData *>())
	{
	}

	JointData::~JointData()
	{
		for (auto &child : m_children)
		{
			delete child;
		}
	}

	void JointData::AddChild(JointData *child)
	{
		m_children.emplace_back(child);
	}
}
