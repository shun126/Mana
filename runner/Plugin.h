/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "common/Setup.h"
#include <string>
#include <memory>
#include <vector>

namespace mana
{
	class VM;

	class Plugin : private Noncopyable
    {
#if defined(MANA_TARGET_WINDOWS)
        using MODULE = HMODULE;
#else
        using MODULE = void*;
#endif
                
    public:
        explicit Plugin(const std::shared_ptr<VM>& vm);

        virtual ~Plugin();

        void Load(const std::string& fileName);
        void Regist(const std::string& directoryName);

    private:
        std::vector<MODULE> mPlugins;
		std::weak_ptr<VM> mVM;
    };
}

#include "Plugin.inl"
