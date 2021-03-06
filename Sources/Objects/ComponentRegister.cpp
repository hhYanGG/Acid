#include "ComponentRegister.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Lights/Light.hpp"
#include "Materials/MaterialDefault.hpp"
#include "Meshes/MeshRender.hpp"
#include "Particles/ParticleSystem.hpp"
#include "Physics/ColliderBox.hpp"
#include "Physics/ColliderCapsule.hpp"
#include "Physics/ColliderCone.hpp"
#include "Physics/ColliderConvexHull.hpp"
#include "Physics/ColliderCylinder.hpp"
#include "Physics/ColliderSphere.hpp"
#include "Physics/Rigidbody.hpp"
#include "Shadows/ShadowRender.hpp"
#include "Skyboxes/MaterialSkybox.hpp"

namespace acid
{
	ComponentRegister::ComponentRegister() :
		m_components(std::map<std::string, ComponentCreate>())
	{
		RegisterComponent<Light>("Light");
		RegisterComponent<MaterialDefault>("MaterialDefault");
		RegisterComponent<MaterialSkybox>("MaterialSkybox");
		RegisterComponent<Mesh>("Mesh");
		RegisterComponent<MeshAnimated>("MeshAnimated");
		RegisterComponent<MeshRender>("MeshRender");
		RegisterComponent<ParticleSystem>("ParticleSystem");
		RegisterComponent<ColliderBox>("ColliderBox");
		RegisterComponent<ColliderCapsule>("ColliderCapsule");
		RegisterComponent<ColliderCone>("ColliderCone");
		RegisterComponent<ColliderConvexHull>("ColliderConvexHull");
		RegisterComponent<ColliderCylinder>("ColliderCylinder");
		RegisterComponent<ColliderSphere>("ColliderSphere");
		RegisterComponent<Rigidbody>("Rigidbody");
		RegisterComponent<ShadowRender>("ShadowRender");
	}

	ComponentRegister::~ComponentRegister()
	{
	}

	IComponent *ComponentRegister::CreateComponent(const std::string &name)
	{
		auto it = m_components.find(name);

		if (it == m_components.end())
		{
			fprintf(stderr, "Could not find registered component: '%s'\n", name.c_str());
			return nullptr;
		}

		return ((*it).second).create();
	}

	bool ComponentRegister::DeregisterComponent(const std::string &name)
	{
		auto component = m_components.find(name);

		if (component == m_components.end())
		{
			return false;
		}

		m_components.erase(component);
		return true;
	}

	std::optional<std::string> ComponentRegister::FindComponentName(IComponent *compare)
	{
		for (auto &component : m_components)
		{
			if (component.second.isSame(compare))
			{
				return component.first;
			}
		}

		return {};
	}
}
