#include "DescriptorsHandler.hpp"

namespace acid
{
	DescriptorsHandler::DescriptorsHandler() :
		m_shaderProgram(nullptr),
		m_descriptorSet(nullptr),
		m_descriptors(std::vector<IDescriptor *>()),
		m_changed(false)
	{
	}

	DescriptorsHandler::DescriptorsHandler(const IPipeline &pipeline) :
		m_shaderProgram(pipeline.GetShaderProgram()),
		m_descriptorSet(new DescriptorSet(pipeline)),
		m_descriptors(std::vector<IDescriptor *>(m_shaderProgram->GetLastDescriptorBinding() + 1)),
		m_changed(true)
	{
	}

	DescriptorsHandler::~DescriptorsHandler()
	{
		delete m_descriptorSet;
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, IDescriptor *descriptor)
	{
		if (m_shaderProgram == nullptr)
		{
			return;
		}

		int location = m_shaderProgram->GetDescriptorLocation(descriptorName);

		if (location == -1)
		{
#ifdef ACID_VERBOSE
			if (m_shaderProgram->ReportedNotFound(descriptorName, true))
			{
				fprintf(stderr, "Could not find descriptor in shader '%s' of name '%s'\n", m_shaderProgram->GetName().c_str(), descriptorName.c_str());
			}
#endif

			return;
		}

		if (m_descriptors.at(location) != descriptor)
		{
			m_descriptors.at(location) = descriptor;
			m_changed = true;
		}
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, UniformHandler *uniformHandler)
	{
		if (m_shaderProgram == nullptr)
		{
			return;
		}

		uniformHandler->Update(m_shaderProgram->GetUniformBlock(descriptorName));
		Push(descriptorName, uniformHandler->GetUniformBuffer());
	}

	bool DescriptorsHandler::Update(const IPipeline &pipeline)
	{
		if (m_shaderProgram != pipeline.GetShaderProgram())
		{
			m_descriptors.clear();
			delete m_descriptorSet;

			m_shaderProgram = pipeline.GetShaderProgram();
			m_descriptors.resize(m_shaderProgram->GetLastDescriptorBinding() + 1);
			m_descriptorSet = new DescriptorSet(pipeline);
			m_changed = false;
			return false;
		}

		if (m_changed)
		{
			m_descriptorSet->Update(m_descriptors);
			m_changed = false;
		}

		return true;
	}
}
