#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>
#include <Textures/Texture.hpp>

using namespace acid;

namespace test
{
	class MaterialTerrain :
		public IMaterial
	{
	private:
		std::shared_ptr<PipelineMaterial> m_material;

		std::shared_ptr<Texture> m_textureR;
		std::shared_ptr<Texture> m_textureG;
	public:
		MaterialTerrain(std::shared_ptr<Texture> textureR = nullptr, std::shared_ptr<Texture> textureG = nullptr);

		~MaterialTerrain();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<Texture> GetTextureR() const { return m_textureR; }

		void SetTextureR(std::shared_ptr<Texture> textureR) { m_textureR = textureR; }

		void TrySetTextureR(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_textureR = Texture::Resource(filename);
			}
		}

		std::shared_ptr<Texture> GetTextureG() const { return m_textureG; }

		void SetTextureG(std::shared_ptr<Texture> textureG) { m_textureG = textureG; }

		void TrySetTextureG(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_textureG = Texture::Resource(filename);
			}
		}

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
