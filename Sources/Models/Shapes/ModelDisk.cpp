#include "ModelDisk.hpp"

#include "Maths/Maths.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	ModelDisk::ModelDisk(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops) :
		Model()
	{
		std::vector<IVertex *> vertices = std::vector<IVertex *>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		for (uint32_t i = 0; i < slices; i++)
		{
			float iDivSlices = static_cast<float>(i) / static_cast<float>(slices);
			float alpha = iDivSlices * 2.0f * PI;
			float xDir = cos(alpha), yDir = sin(alpha);

			for (uint32_t j = 0; j < loops + 1; j++)
			{
				float jDivLoops = static_cast<float>(j) / static_cast<float>(loops);
				float radius = innerRadius + jDivLoops * (outerRadius - innerRadius);

				VertexModel *vertex = new VertexModel();
				vertex->m_normal.m_x = 0.0f;
				vertex->m_normal.m_y = 1.0f;
				vertex->m_normal.m_z = 0.0f;
				vertex->m_uv.m_x = 1.0f - iDivSlices;
				vertex->m_uv.m_y = 1.0f - jDivLoops;
				vertex->m_position.m_x = radius * xDir;
				vertex->m_position.m_y = 0.0f;
				vertex->m_position.m_z = radius * yDir;

				vertices.emplace_back(vertex);
			}
		}

		for (uint32_t i = 0; i < slices; i++)
		{
			for (uint32_t j = 0; j < loops; j++)
			{
				uint32_t first = i * (loops + 1) + j;
				uint32_t second = (first + loops + 1) % (slices * (loops + 1));

				indices.emplace_back(first);
				indices.emplace_back(second + 1);
				indices.emplace_back(second);

				indices.emplace_back(first);
				indices.emplace_back(first + 1);
				indices.emplace_back(second + 1);
			}
		}

		std::reverse(indices.begin(), indices.end());
		Model::Set(vertices, indices, ToFilename(innerRadius, outerRadius, slices, loops));
	}

	ModelDisk::~ModelDisk()
	{
	}

	std::string ModelDisk::ToFilename(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops)
	{
		return "Disk_" + std::to_string(innerRadius) + "_" + std::to_string(outerRadius) + "_" +
			std::to_string(slices) + "_" + std::to_string(loops);
	}
}
