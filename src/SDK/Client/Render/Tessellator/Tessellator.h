#pragma once

#include "VertextFormat.h"
#include "MaterialPtr.h"
#include "../../../../Utils/Memory/Memory.hpp"
#include "../../../../Utils/Utils.hpp"

class ScreenContext;

class Tessellator 
{
private:
	char pad_0000[0x128];
	Vec3<float> transformOffset; // 0x128

public:
	void addPostTransformOffset(int x, int y, int z) 
	{
		addPostTransformOffset((float)x, (float)y, (float)z);
	}
	void addPostTransformOffset(float x, float y, float z) 
	{
		this->transformOffset = this->transformOffset.add(Vec3<float>(x, y, z));
	};

	inline void begin(VertextFormat format, int reservedVerticies = 0) 
	{
		using tess_begin_t = void(__fastcall*)(Tessellator*, VertextFormat, bool);
		static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(Memory::findSig("48 89 5c 24 ? 55 48 83 ec ? 80 b9 ? ? ? ? ? 45 0f b6 d1"));
		tess_begin(this, format, reservedVerticies);
	}
	inline void vertex(float x, float y, float z) 
	{
		using tess_vertex_t = void(__fastcall*)(Tessellator*, float, float, float);
		static tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(Memory::findSig("40 57 48 81 ec ? ? ? ? 0f 29 7C 24"));
		tess_vertex(this, x, y, z);
	}
	inline void color(float r, float g, float b, float a) 
	{
		using tess_color_t = void(__fastcall*)(Tessellator*, float, float, float, float);
		static tess_color_t tess_color = reinterpret_cast<tess_color_t>(Memory::findSig("80 b9 ? ? ? ? ? 4c 8b c1 75"));
		tess_color(this, r, g, b, a);
	}
	inline void setRotation(float angle, Vec3<float> pivot)
	{
		using tess_setRotation_t = void(__fastcall*)(Tessellator*, float, Vec3<float>);
		static tess_setRotation_t tess_setRotation = reinterpret_cast<tess_setRotation_t>(Memory::findSig("40 53 48 81 ec ? ? ? ? f3 41 0f 10 ?"));
		tess_setRotation(this, angle, pivot);
	}
	inline void resetTransform(bool a1) 
	{
		using tess_resetTransform_t = void(__fastcall*)(Tessellator*, bool);
		static tess_resetTransform_t tess_resetTransform = reinterpret_cast<tess_resetTransform_t>(Memory::findSig("40 53 48 81 ec ? ? ? ? c6 81 ? ? ? ? ? 48 8b d9 84 d2"));
		tess_resetTransform(this, a1);
	}
	inline void renderMeshImmediately(ScreenContext* ctx, MaterialPtr* material) 
	{
		using meshHelper_renderImm_t = void(__fastcall*)(ScreenContext*, Tessellator*, MaterialPtr*);
		static meshHelper_renderImm_t meshHelper_renderImm = reinterpret_cast<meshHelper_renderImm_t>(Memory::findSig("40 55 53 56 57 41 56 48 8d ac 24 ? ? ? ? 48 81 ec ? ? ? ? 49 8b f0"));
		meshHelper_renderImm(ctx, this, material);
	}
};