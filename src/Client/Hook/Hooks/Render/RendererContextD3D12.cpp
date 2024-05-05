#include "SwapchainHook.hpp"
#include "../../../GUI/D2D.hpp"
#include "../../../Events/Render/RenderEvent.hpp"
#include "../../../Events/EventHandler.hpp"
#include "d2d1.h"
#include "../../../Client.hpp"
#include "../../../Module/Modules/CPS/CPSListener.hpp"
#include <d3d11on12.h>
#include <wrl/client.h>
#include <algorithm>
#include <windows.h>
#include <iostream>
#include <Psapi.h>
#include "RendererContextD3D12.hpp"

static std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
static std::chrono::steady_clock::time_point previousFrameTime = std::chrono::high_resolution_clock::now();

void RendererContextD3D12::callback(void *_this, void *_render, void *_formal,void *_textVideoMemBlitter) {
    SwapchainHook::rendered = false;
    IDXGISwapChain3* swapChain = hat::member_at<IDXGISwapChain3*>(_this, 0x338);

    SwapchainHook::swapchain = swapChain;

    std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - start;
    MC::frames += 1;


    if (elapsed.count() >= 0.5f) {
        // Calculate frame rate based on elapsed time
        MC::fps = static_cast<int>(MC::frames / elapsed.count());
        // Reset frame counter and update start time
        MC::frames = 0;
        start = std::chrono::high_resolution_clock::now();
    }

    constexpr float targetFrameRate = 60.0f;

// Measure the elapsed frame time
    std::chrono::duration<float> frameTime = std::chrono::high_resolution_clock::now() - previousFrameTime;
    previousFrameTime = std::chrono::high_resolution_clock::now();

// Calculate the current frame rate
    float currentFrameRate = 1.0f / frameTime.count();

// Calculate the frame factor as a percentage
    FlarialGUI::frameFactor = targetFrameRate / currentFrameRate;

// Limit the frame factor to a maximum of 1.0
    FlarialGUI::frameFactor = std::min(FlarialGUI::frameFactor, 1.0f);

    if (!SwapchainHook::init) {
        if (SwapchainHook::queue == nullptr) {

            ID3D12Device5 *d3d12device3;

            if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(&d3d12device3)))) {
                Logger::debug("[SwapChain] Removed d3d12 device");
                d3d12device3->RemoveDevice();

                RendererContextD3D12Original(_this, _render, _formal, _textVideoMemBlitter);
            }

            Logger::debug("[SwapChain] Not a DX12 device, running dx11 procedures");

            const D2D1_CREATION_PROPERTIES properties
                    {
                            D2D1_THREADING_MODE_MULTI_THREADED,
                            D2D1_DEBUG_LEVEL_NONE,
                            D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS
                    };

            IDXGISurface1 *eBackBuffer;
            swapChain->GetBuffer(0, IID_PPV_ARGS(&eBackBuffer));

            D2D1CreateDeviceContext(eBackBuffer, properties, &D2D::context);

            D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
                    D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), 96.0, 96.0);
            D2D::context->CreateBitmapFromDxgiSurface(eBackBuffer, props, &SwapchainHook::D2D1Bitmap);

            Memory::SafeRelease(eBackBuffer);

            SwapchainHook::init = true;


        } else {

            ID3D12Device5 *device;

            if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(&device))) &&
                kiero::getRenderType() == kiero::RenderType::D3D12) {
                ID3D11Device *d3d11device;
                D3D11On12CreateDevice(device,
                                      D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0,
                                      (IUnknown **) &SwapchainHook::queue, 1, 0, &d3d11device, &SwapchainHook::context,
                                      nullptr);

                d3d11device->QueryInterface(IID_PPV_ARGS(&SwapchainHook::d3d11On12Device));

                D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS;
                ID2D1Factory7 *d2dFactory;
                D2D1_FACTORY_OPTIONS factoryOptions{};
                D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory7), &factoryOptions,
                                  (void **) &d2dFactory);

                IDXGIDevice *dxgiDevice;
                SwapchainHook::d3d11On12Device->QueryInterface(__uuidof(IDXGIDevice), (void **) &dxgiDevice);

                ID2D1Device6 *device2;
                d2dFactory->CreateDevice(dxgiDevice, &device2);

                device2->CreateDeviceContext(deviceOptions, &D2D::context);

                Logger::debug("[SwapChain] Prepared.");

                DXGI_SWAP_CHAIN_DESC1 swapChainDescription;
                swapChain->GetDesc1(&swapChainDescription);

                SwapchainHook::bufferCount = swapChainDescription.BufferCount;

                SwapchainHook::DXGISurfaces.resize(SwapchainHook::bufferCount, nullptr);
                SwapchainHook::D3D11Resources.resize(SwapchainHook::bufferCount, nullptr);
                SwapchainHook::D2D1Bitmaps.resize(SwapchainHook::bufferCount, nullptr);

                auto dpi = (float) GetDpiForSystem();

                D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
                        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpi, dpi);

                D3D12_DESCRIPTOR_HEAP_DESC heapDescriptorBackBuffers = {};
                heapDescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
                heapDescriptorBackBuffers.NumDescriptors = SwapchainHook::bufferCount;
                heapDescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
                heapDescriptorBackBuffers.NodeMask = 1;

                device->CreateDescriptorHeap(&heapDescriptorBackBuffers,
                                             IID_PPV_ARGS(&SwapchainHook::D3D12DescriptorHeap));

                uintptr_t rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = SwapchainHook::D3D12DescriptorHeap->GetCPUDescriptorHandleForHeapStart();

                for (int i = 0; i < SwapchainHook::bufferCount; i++) {

                    ID3D12Resource *backBufferPtr;
                    swapChain->GetBuffer(i, IID_PPV_ARGS(&backBufferPtr));
                    device->CreateRenderTargetView(backBufferPtr, nullptr, rtvHandle);
                    rtvHandle.ptr += rtvDescriptorSize;


                    D3D11_RESOURCE_FLAGS d3d11_flags = {D3D11_BIND_RENDER_TARGET};

                    SwapchainHook::d3d11On12Device->CreateWrappedResource(backBufferPtr, &d3d11_flags,
                                                                          D3D12_RESOURCE_STATE_RENDER_TARGET,
                                                                          D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(
                                                                                  &SwapchainHook::D3D11Resources[i]));
                    SwapchainHook::D3D11Resources[i]->QueryInterface(&SwapchainHook::DXGISurfaces[i]);

                    SwapchainHook::D2D1Bitmaps[i] = nullptr; // Initialize to nullptr

                    D2D::context->CreateBitmapFromDxgiSurface(SwapchainHook::DXGISurfaces[i], props,
                                                              &(SwapchainHook::D2D1Bitmaps[i]));
                    Memory::SafeRelease(backBufferPtr);

                }

                Memory::SafeRelease(device);
                Memory::SafeRelease(device2);
                Memory::SafeRelease(d3d11device);
                Memory::SafeRelease(dxgiDevice);
                Memory::SafeRelease(d2dFactory);

                SwapchainHook::init = true;
            }
        }


    } else {

    }

    RendererContextD3D12Original(_this, _render, _formal, _textVideoMemBlitter);
}

void RendererContextD3D12::enableHook() {
    this->autoHook((void *) callback, (void **) &RendererContextD3D12Original);
}
