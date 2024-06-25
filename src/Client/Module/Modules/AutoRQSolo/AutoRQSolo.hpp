#pragma once

#include "../Module.hpp"
#include "../../../Events/EventHandler.hpp"
#include "AutoRQSoloListener.hpp"


class AutoRQSolo : public Module {

public:


    AutoRQSolo() : Module("Auto RE Q Solo", "Automatically requeues into games when they end or if you die/finish a game (hive) pls don't use in party's",
                      R"(\Flarial\assets\re-q-solo.png)", "") {

        Module::setup();

    };

    void onEnable() override {
        EventHandler::registerListener(new AutoRQSoloListener("AutoRQSolo", this));
        Module::onEnable();
    }

    void onDisable() override {
        EventHandler::unregisterListener("AutoRQSolo");
        Module::onDisable();
    }

    void defaultConfig() override {
        if (settings.getSettingByName<std::string>("command") == nullptr)
            settings.addSetting("command", (std::string)"");
    }

    void settingsRender() override {

    }
};

