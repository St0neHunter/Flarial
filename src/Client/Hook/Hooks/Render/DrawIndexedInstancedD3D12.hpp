#include <dxgi1_6.h>
#include <d2d1_3.h>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include "kiero.h"
#include "../Hook.hpp"

class DrawIndexedInstancedD3D12 : public Hook {
private:
    static void listCallback(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex);

public:

    DrawIndexedInstancedD3D12();

    void enableHook() override;
};