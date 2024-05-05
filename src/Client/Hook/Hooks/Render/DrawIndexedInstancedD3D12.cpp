#include "DrawIndexedInstancedD3D12.hpp"
#include "../../../GUI/D2D.hpp"
#include "SwapchainHook.hpp"
#include "../../../Client.hpp"

typedef void(__fastcall *original)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex);

original funcOriginal = nullptr;

void DrawIndexedInstancedD3D12::enableHook() {

    if (!Client::settings.getSettingByName<bool>("killdx")->value) {
        if (kiero::getRenderType() == kiero::RenderType::D3D12)
            this->manualHook((void *) kiero::getMethodsTable()[85], (void **) &DrawIndexedInstancedD3D12::listCallback,
                             (void **) &funcOriginal);
        else if (kiero::getRenderType() == 0) {
            kiero::init(kiero::RenderType::D3D11);
            SwapchainHook::queue = nullptr;
        }
    } else {
        SwapchainHook::queue = nullptr;
    }

}

DrawIndexedInstancedD3D12::DrawIndexedInstancedD3D12() : Hook("DrawIndexedInstancedD3D12", "") {}

void DrawIndexedInstancedD3D12::listCallback(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex) {
    if(IndexCount == 54) {
        if (D2D::context && !SwapchainHook::rendered) {
            if (FlarialGUI::writeFactory == nullptr) {
                DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                                    reinterpret_cast<IUnknown **>(&FlarialGUI::writeFactory));
            }

            if (D2D::context != nullptr && !Client::disable && SwapchainHook::init && FlarialGUI::writeFactory != nullptr) {

                if (SwapchainHook::queue != nullptr) {

                    SwapchainHook::currentBitmap = SwapchainHook::swapchain->GetCurrentBackBufferIndex();

                    ID3D11Resource *resource = SwapchainHook::D3D11Resources[SwapchainHook::currentBitmap];
                    SwapchainHook::d3d11On12Device->AcquireWrappedResources(&resource, 1);

                    D2D::context->SetTarget(SwapchainHook::D2D1Bitmaps[SwapchainHook::currentBitmap]);

                    /* Blur Stuff */

                    if (ModuleManager::doesAnyModuleHave("BlurEffect") &&
                        Client::settings.getSettingByName<float>("blurintensity")->value > 1 ||
                        !FlarialGUI::notifications.empty() &&
                        Client::settings.getSettingByName<float>("blurintensity")->value > 1) {
                        ID2D1Bitmap *bitmap = nullptr;

                        if (FlarialGUI::blur == nullptr) {
                            D2D::context->CreateEffect(CLSID_D2D1GaussianBlur, &FlarialGUI::blur);
                        }

                        if (SwapchainHook::queue != nullptr)
                            FlarialGUI::CopyBitmap(SwapchainHook::D2D1Bitmaps[SwapchainHook::currentBitmap], &bitmap);
                        else FlarialGUI::CopyBitmap(SwapchainHook::D2D1Bitmap, &bitmap);

                        FlarialGUI::blur->SetInput(0, bitmap);

                        // Set blur intensity
                        FlarialGUI::blur->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
                        FlarialGUI::blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION,
                                                   Client::settings.getSettingByName<float>("blurintensity")->value);
                        FlarialGUI::blur->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION,
                                                   D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);


                        ID2D1Image *image;
                        FlarialGUI::blur->GetOutput(&image);
                        D2D1_IMAGE_BRUSH_PROPERTIES props = D2D1::ImageBrushProperties(
                                D2D1::RectF(0, 0, MC::windowSize.x, MC::windowSize.y));
                        D2D::context->CreateImageBrush(image, props, &FlarialGUI::blurbrush);


                        Memory::SafeRelease(image);
                        Memory::SafeRelease(bitmap);
                    }

                    /* Blur End */
                    D2D::context->BeginDraw();

                    MC::windowSize = Vec2<float>(D2D::context->GetSize().width, D2D::context->GetSize().height);

                    RenderEvent event{};
                    EventHandler::onRender(event);


                    D2D::context->EndDraw();

                    D2D::context->SetTarget(nullptr);
                    SwapchainHook::d3d11On12Device->ReleaseWrappedResources(&resource, 1);
                    SwapchainHook::context->Flush();

                } else {

                    /* Blur Stuff */

                    if (ModuleManager::doesAnyModuleHave("BlurEffect") &&
                        Client::settings.getSettingByName<float>("blurintensity")->value > 1 ||
                        !FlarialGUI::notifications.empty() &&
                        Client::settings.getSettingByName<float>("blurintensity")->value > 1) {
                        ID2D1Bitmap *bitmap = nullptr;

                        if (FlarialGUI::blur == nullptr) {
                            D2D::context->CreateEffect(CLSID_D2D1GaussianBlur, &FlarialGUI::blur);
                        }

                        if (SwapchainHook::queue != nullptr)
                            FlarialGUI::CopyBitmap(SwapchainHook::D2D1Bitmaps[SwapchainHook::currentBitmap], &bitmap);
                        else FlarialGUI::CopyBitmap(SwapchainHook::D2D1Bitmap, &bitmap);

                        FlarialGUI::blur->SetInput(0, bitmap);

                        // Set blur intensity
                        FlarialGUI::blur->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
                        FlarialGUI::blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION,
                                                   Client::settings.getSettingByName<float>("blurintensity")->value);
                        FlarialGUI::blur->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION,
                                                   D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);


                        ID2D1Image *image;
                        FlarialGUI::blur->GetOutput(&image);
                        D2D1_IMAGE_BRUSH_PROPERTIES props = D2D1::ImageBrushProperties(
                                D2D1::RectF(0, 0, MC::windowSize.x, MC::windowSize.y));
                        D2D::context->CreateImageBrush(image, props, &FlarialGUI::blurbrush);


                        Memory::SafeRelease(image);
                        Memory::SafeRelease(bitmap);
                    }
                    /* Blur End */


                    MC::windowSize = Vec2<float>(D2D::context->GetSize().width, D2D::context->GetSize().height);

                    RenderEvent event;
                    EventHandler::onRender(event);
                    D2D::context->EndDraw();
                }

                Memory::SafeRelease(FlarialGUI::blurbrush);
                Memory::SafeRelease(FlarialGUI::blur);
            }
            SwapchainHook::rendered = true;
        }
    }
    return funcOriginal(dCommandList, IndexCount, InstanceCount, StartIndex, BaseVertex);
}
