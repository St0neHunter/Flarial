#pragma once

#include "../Hook.hpp"
#include "../../../../SDK/Client/Actor/Actor.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"

class RendererContextD3D12 : public Hook {

private:
    static void callback(void *_this, void *_render, void *_formal,void *_textVideoMemBlitter);

public:

    typedef void(__thiscall *original)(void *_this, void *_render, void *_formal,void *_textVideoMemBlitter);

    static inline original RendererContextD3D12Original = nullptr;

    RendererContextD3D12() : Hook("bgfx::d3d12::RendererContextD3D12::submit", GET_SIG("RendererContextD3D12::submit")) {}

    void enableHook() override;
};