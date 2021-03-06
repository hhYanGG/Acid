#pragma once

#include "Engine/Engine.hpp"

typedef struct ALCdevice_struct ALCdevice;

typedef struct ALCcontext_struct ALCcontext;

namespace acid
{
	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class ACID_EXPORT Audio :
		public IModule
	{
	private:
		ALCdevice *m_alDevice;
		ALCcontext *m_alContext;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Audio *Get()
		{
			return Engine::Get()->GetModule<Audio>();
		}

		/// <summary>
		/// Creates a new audio module.
		/// </summary>
		Audio();

		/// <summary>
		/// Deconstructor for the audio module.
		/// </summary>
		~Audio();

		void Update() override;

		ACID_HIDDEN static std::string StringifyResultAl(const int &result);

		ACID_HIDDEN static void CheckAl(const int &result);

		ACID_HIDDEN ALCdevice *GetDevice() const { return m_alDevice; }

		ACID_HIDDEN ALCcontext *GetContext() const { return m_alContext; }
	};
}
