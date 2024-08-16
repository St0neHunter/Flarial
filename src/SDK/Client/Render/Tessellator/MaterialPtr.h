#pragma once

#include "../TexturePtr.hpp"
#include "../../../../Utils/Memory/Memory.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"
#include "../../Core/HashedString.hpp"

namespace mce {
    class MaterialPtr {
    public:
        static MaterialPtr *createMaterial(HashedString const &materialName) {
            static void *materialCreator = nullptr;

            if (materialCreator == nullptr) {
                // Sig returns 6 addresses, all of them point to the same offset
                uintptr_t sigOffset = Memory::findSig(GET_SIG("Tessellator::createMaterial"));
                int offset = *reinterpret_cast<int *>(sigOffset + 3);
                materialCreator = reinterpret_cast<void *>(sigOffset + offset + 7);
            }

            static int off = GET_OFFSET("createMaterial");
            return Memory::CallVFuncI<MaterialPtr *>(off, materialCreator, materialName);
        }
    };
}