#pragma once


#include "../../../../SDK/SDK.hpp"
#include <format>
#include "../../../Events/Listener.hpp"
#include "../../../Events/Input/KeyEvent.hpp"
#include "../Module.hpp"
#include "../../../GUI/Engine/Engine.hpp"
#include "../../../../SDK/SDK.hpp"
#include "MovableChatListener.hpp"
#include "../../Manager.hpp"
#include "../ClickGUI/ClickGUIRenderer.hpp"
#include "../../../../Utils/Render/PositionUtils.hpp"
#include <Windows.h>
#include <chrono>

// TODO: fix and rename stuff

class MovableChatListener : public Listener {
public:

    Module *module;
    Vec2<float> currentPos;
    bool enabled = true;
    static inline Vec2<float> oriXy = Vec2<float>{0.0f, 0.0f};

    void onRender(RenderEvent &event) override {

        if (!module->isEnabled()) enabled = false;

        if (ClientInstance::getTopScreenName() == "hud_screen" &&
            module->isEnabled() ||
            ClientInstance::getTopScreenName() == "pause_screen" &&
            module->isEnabled()) {

            if (!enabled && FlarialGUI::inMenu) {
                FlarialGUI::Notify("To change the position of the chat, Please click " +
                                   ModuleManager::getModule("ClickGUI")->settings.getSettingByName<std::string>(
                                           "editmenubind")->value);
                enabled = true;
            }

            float width = Constraints::RelativeConstraint(0.0366, "height", true) *
                          module->settings.getSettingByName<float>("uiscale")->value;
            float height = Constraints::RelativeConstraint(0.015, "height", true) *
                           module->settings.getSettingByName<float>("uiscale")->value;


            Vec2<float> settingperc = Vec2<float>(module->settings.getSettingByName<float>("percentageX")->value,
                                                  module->settings.getSettingByName<float>("percentageY")->value);

            if (settingperc.x != 0)
                currentPos = Vec2<float>(settingperc.x * MC::windowSize.x,
                                         settingperc.y * MC::windowSize.y);
            else if (settingperc.x == 0 and MovableChatListener::oriXy.x != 0.0f)
                currentPos = Vec2<float>{MovableChatListener::oriXy.x, MovableChatListener::oriXy.y};

            if (ClickGUIRenderer::editmenu)
                FlarialGUI::SetWindowRect(currentPos.x, currentPos.y, width, height, 21);

            Vec2<float> vec2 = FlarialGUI::CalculateMovedXY(currentPos.x, currentPos.y, 21, width, height);


            currentPos.x = vec2.x;
            currentPos.y = vec2.y;

            Vec2<float> percentages = Constraints::CalculatePercentage(currentPos.x, currentPos.y);

            module->settings.setValue("percentageX", percentages.x);
            module->settings.setValue("percentageY", percentages.y);

            if (ClickGUIRenderer::editmenu) {
                FlarialGUI::RoundedRect(currentPos.x, currentPos.y, D2D1::ColorF(D2D1::ColorF::White, 0.2f), width, height);
                FlarialGUI::UnsetWindowRect();
            }
        } else {
            enabled = false;
        }
    }


    void onSetupAndRender(SetupAndRenderEvent &event) override {

        if(this->module->isEnabled())


        if (SDK::screenView->VisualTree->root->LayerName == "hud_screen") {

            SDK::screenView->VisualTree->root->forEachControl([this](const std::shared_ptr<UIControl>& control) {

                if (control->LayerName == "chat_panel" && module->isEnabled()) {


                    if (MovableChatListener::oriXy.x == 0.0f) {
                        MovableChatListener::oriXy.x = control->x;
                        MovableChatListener::oriXy.y = control->y;
                    }

                    Vec2<float> scaledPos = PositionUtils::getScaledPos(currentPos);

                    control->x = scaledPos.x + 5;
                    control->y = scaledPos.y;

                    control->scale = module->settings.getSettingByName<float>("uiscale")->value + 100;

                }

            });
        }
    }

public:
    explicit MovableChatListener(const char string[5], Module *module) {
        this->name = string;
        this->module = module;
        this->currentPos = Vec2<float>(0, 0);
    }

};