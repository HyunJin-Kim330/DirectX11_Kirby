#pragma once

#include "Geometry.h"


class GeometryMaker
{
public:
	static void CreateRectangle(Geometry<VertexTextureData>* geometry, float width, float height);
	static void CreateRectangleMidPointOnMidst(Geometry<VertexTextureData>* geometry, float width, float height);
	static void CreateRectangleMidPointOnRight(Geometry<VertexTextureData>* geometry, float width, float height);
	static void CreateRectangleMidPointOnLeft(Geometry<VertexTextureData>* geometry, float width, float height);
	static void CreateRectangle(Geometry<VertexTextureData>* geometry);
};
