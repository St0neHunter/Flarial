﻿#include "Utils.hpp"
#include "Logger/Logger.hpp"
#include "../Client/GUI/Engine/Engine.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

std::string Utils::getRoamingPath() {
    char *path = nullptr;
    size_t length;

    _dupenv_s(&path, &length, "appdata");

    if (path == nullptr)
        return "";

    return std::string(path) + R"(\..\Local\Packages\Microsoft.MinecraftUWP_8wekyb3d8bbwe\RoamingState)"; // Microsoft.MinecraftWindowsBeta_8wekyb3d8bbwe Microsoft.MinecraftUWP_8wekyb3d8bbwe
}

bool Utils::hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}


std::string Utils::removeColorCodes(const std::string &input) {
    std::string result;
    std::string newinput = input;

    bool skipNext = false;

    for (wchar_t c: FlarialGUI::to_wide(newinput)) {
        if (skipNext) {
            skipNext = false;
        } else if (c == L'§') {
            skipNext = true;
        } else {
            result += (char)c;
        }
    }

    result.erase(std::prev(result.end()));


    return result;
}

std::string Utils::removeNonAlphanumeric(const std::string &input) {
    std::string result;
    std::copy_if(input.begin(), input.end(), std::back_inserter(result), [](char c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == ' ';

    });
    return result;
}

std::string Utils::remomveNonNumeric(const std::string &input) {
    std::string result;
    std::copy_if(input.begin(), input.end(), std::back_inserter(result), [](char c) {
        return (c >= '0' && c <= '9') || c == '.';
    });
    return result;
}

std::string Utils::getKeyAsString(int key, bool isCapital, bool isKeybind) {
    if (key == 32) { // Spacebar key
        return " ";
    }

    if (isKeybind) {
        switch (key) {
            case 18:
                return "ALT";
            case 17:
                return "CTRL";
            case 16:
                return "SHIFT";
            case 9:
                return "TAB";
            case 45:
                return "INS";
            case 46:
                return "DEL";
            case 36:
                return "HOME";
            case 35:
                return "END";
            case 33:
                return "PG UP";
            case 34:
                return "PG DOWN";
            case 112:
                return "F1";
            case 113:
                return "F2";
            case 114:
                return "F3";
            case 115:
                return "F4";
            case 116:
                return "F5";
            case 117:
                return "F6";
            case 118:
                return "F7";
            case 119:
                return "F8";
            case 120:
                return "F9";
            case 121:
                return "F10";
            case 122:
                return "F11";
            case 123:
                return "F12";
            default:
                break;
        }
    }

    if (isCapital) {
        switch (key) {
            // A - Z
            case 48:
                return "0";
            case 49:
                return "1";
            case 50:
                return "2";
            case 51:
                return "3";
            case 52:
                return "4";
            case 53:
                return "5";
            case 54:
                return "6";
            case 55:
                return "7";
            case 56:
                return "8";
            case 57:
                return "9";
            case 65:
                return "A";
            case 66:
                return "B";
            case 67:
                return "C";
            case 68:
                return "D";
            case 69:
                return "E";
            case 70:
                return "F";
            case 71:
                return "G";
            case 72:
                return "H";
            case 73:
                return "I";
            case 74:
                return "J";
            case 75:
                return "K";
            case 76:
                return "L";
            case 77:
                return "M";
            case 78:
                return "N";
            case 79:
                return "O";
            case 80:
                return "P";
            case 81:
                return "Q";
            case 82:
                return "R";
            case 83:
                return "S";
            case 84:
                return "T";
            case 85:
                return "U";
            case 86:
                return "V";
            case 87:
                return "W";
            case 88:
                return "X";
            case 89:
                return "Y";
            case 90:
                return "Z";
            case 188:
                return "<";
            case 190:
                return ">";
            case 192:
                return "~";
            case 40:
                return "(";
            case 41:
                return ")";
            case 37:
                return "%";
            case 91:
                return "{";
            case 93:
                return "}";
            case 220:
                return "|";
            case 191:
                return "?";
            case 186:
                return ":";
            case 187:
                return "+";
            case 219:
                return "{";
            case 221:
                return "}";
            default:
                return "";
        }
    } else {
        switch (key) {
            // a - z
            case 48:
                return "0";
            case 49:
                return "1";
            case 50:
                return "2";
            case 51:
                return "3";
            case 52:
                return "4";
            case 53:
                return "5";
            case 54:
                return "6";
            case 55:
                return "7";
            case 56:
                return "8";
            case 57:
                return "9";
            case 65:
                return "a";
            case 66:
                return "b";
            case 67:
                return "c";
            case 68:
                return "d";
            case 69:
                return "e";
            case 70:
                return "f";
            case 71:
                return "g";
            case 72:
                return "h";
            case 73:
                return "i";
            case 74:
                return "j";
            case 75:
                return "k";
            case 76:
                return "l";
            case 77:
                return "m";
            case 78:
                return "n";
            case 79:
                return "o";
            case 80:
                return "p";
            case 81:
                return "q";
            case 82:
                return "r";
            case 83:
                return "s";
            case 84:
                return "t";
            case 85:
                return "u";
            case 86:
                return "v";
            case 87:
                return "w";
            case 88:
                return "x";
            case 89:
                return "y";
            case 90:
                return "z";
            case 188:
                return ",";
            case 190:
                return ".";
            case 192:
                return "`";
            case 219:
                return "[";
            case 221:
                return "]";
            case 220:
                return "\\";
            case 191:
                return "/";
            case 187:
                return "=";
            case 189:
                return "-";
            case 186:
                return (std::string) ";";
            default:
                return "";
        }
    }
}

int Utils::getStringAsKey(const std::string &str) {
    if (str == "ALT" || str == "alt") return 18;
    if (str == "CTRL" || str == "ctrl") return 17;
    if (str == "SHIFT" || str == "shift") return 16;
    if (str == "INS" || str == "ins") return 45;
    if (str == "DEL" || str == "del") return 46;
    if (str == "HOME" || str == "home") return 36;
    if (str == "PG UP" || str == "pg up") return 33;
    if (str == "PG DOWN" || str == "pg down") return 34;
    if (str == "TAB") return 9;
    if (str == "{") return 219;
    if (str == "}") return 221;
    if (str == "+") return 187;
    if (str == ":") return 186;
    if (str == "?") return 191;
    if (str == "|") return 220;
    if (str == "`" || str == "~") return 192;
    if (str == " ") return 32;
    if (str == "F1" || str == "f1") return 112;
    if (str == "F2" || str == "f2") return 113;
    if (str == "F3" || str == "f3") return 114;
    if (str == "F4" || str == "f4") return 115;
    if (str == "F5" || str == "f5") return 116;
    if (str == "F6" || str == "f6") return 117;
    if (str == "F7" || str == "f7") return 118;
    if (str == "F8" || str == "f8") return 119;
    if (str == "F9" || str == "f9") return 120;
    if (str == "F10" || str == "f10") return 121;
    if (str == "F11" || str == "f11") return 122;
    if (str == "F12" || str == "f12") return 123;
    char c = str[0];

    return static_cast<int>(std::toupper(c));
}

 std::vector<int> Utils::getStringAsKeys(const std::string& str) {
    std::vector<int> keys;

    if (str.empty()) {
        // Empty string
        return keys;
    }

    std::stringstream iss(str);
    std::string token;
    while (std::getline(iss, token, '+')) {
        int key = Utils::getStringAsKey(token);
        if (key != -1) {
            keys.push_back(key);
        }
    }

    return keys;
}

bool Utils::CursorInEllipse(float ellipseX, float ellipseY, float radiusX, float radiusY) {
    float mouseX = MC::mousePos.x;
    float mouseY = MC::mousePos.y;

    float normalizedX = (mouseX - ellipseX) / radiusX;
    float normalizedY = (mouseY - ellipseY) / radiusY;

    return (normalizedX * normalizedX + normalizedY * normalizedY) <= 1.0f;
}
