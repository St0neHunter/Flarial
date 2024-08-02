#include "../../../../Engine.hpp"
#include "../../../../../../Client.hpp"

inline float GetColorComponent(ImU32 color, int shift) {
    return ((color >> shift) & 0xFF) / 255.0f;
}

void RoundedRectBorder(Vec2<float> Position, Vec2<float> Size, ImColor Colour, float Thickness, float Rounding, int Flags) {
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(Position.x, Position.y), ImVec2(Position.x + Size.x, Position.y + Size.y), Colour, Rounding, Flags, Thickness);
}

void FlarialGUI::ShadowRect(Vec2<float> pos, Vec2<float> size, D2D_COLOR_F color, float rounding, int shadowSize)
{
    if (isInScrollView) pos.y += scrollpos;

    ImColor shadowColor = ImColor(color.r, color.g, color.b, color.a);
	shadowColor.Value.w *= .5f;

    for (int i = 0; i < shadowSize; i++)
    {
        float progress = (float)i / shadowSize;
        float alphaFactor = (1.0f - progress) * (1.0f - progress); //took help from gpt for alpha factor

        float shadowR = GetColorComponent(shadowColor, IM_COL32_R_SHIFT);
        float shadowG = GetColorComponent(shadowColor, IM_COL32_G_SHIFT);
        float shadowB = GetColorComponent(shadowColor, IM_COL32_B_SHIFT);
        float shadowA = GetColorComponent(shadowColor, IM_COL32_A_SHIFT) * alphaFactor;

        ImU32 fadedShadowColor = ImColor(shadowR, shadowG, shadowB, shadowA);

        Vec2 offset = Vec2(progress * shadowSize, progress * shadowSize);

        //ImGui::GetBackgroundDrawList()->AddRect(ImVec2(pos.x - offset.x, pos.y - offset.y), ImVec2(pos.x + size.x + (offset.x * 2), pos.y + size.y + (offset.y * 2)), fadedShadowColor, rounding + progress * shadowSize, 240, 2);

		RoundedRectBorder(pos - offset, size + offset + offset, fadedShadowColor, 2.0f, rounding + progress * shadowSize, 240);
    }
    //ImGui::GetBackgroundDrawList()->AddRect(ImVec2(pos.x, pos.y), ImVec2(size.x + pos.y, size.y + pos.y), shadowColor, rounding, 240, 1);
	RoundedRectBorder(pos, size, ImColor(color.r, color.g, color.b, color.a), 1, rounding, 240);

    /*

    color.a *= .5f;

    for (int i = 0; i < shadowSize; i++)
    {
        float progress = (float)i / shadowSize;
        float alphaFactor = (1.0f - progress) * (1.0f - progress); //took help from gpt for alpha factor

        ImColor fadedShadowColor = ImColor(color.r, color.g, color.b, color.a * alphaFactor);

        Vec2 offset = Vec2(progress * shadowSize, progress * shadowSize);

        ImGui::GetBackgroundDrawList()->AddRect(ImVec2(pos.x - (offset.x), pos.y - (offset.y)), ImVec2(pos.x + size.x + (offset.x * 2), pos.y + size.y + (offset.y * 2)), fadedShadowColor, rounding + progress * shadowSize, 0, 2);

        //FlarialGUI::RoundedHollowRect(pos.x - offset.x, pos.y - offset.y, 2.0f, fadedShadowColor, size.x + offset.x*2, size.y + offset.y*2, rounding + progress * shadowSize, rounding + progress * shadowSize);
        
        //RoundedRectBorder(pos - offset, size + offset + offset, fadedShadowColor, 2.0f, rounding + progress * shadowSize);
    }

    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), D2DColorToImColor(color), rounding, 0, 2);

    //FlarialGUI::RoundedHollowRect(pos.x, pos.y, shadowSize, color, size.x, size.y, rounding, rounding);
    //RoundedRectBorder(pos, size, color, 1, rounding);

    */
}