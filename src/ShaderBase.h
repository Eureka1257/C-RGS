#pragma once
#include"Math.h"
#include<string>

namespace RGS 
{
	struct VertexBase
	{
		Vector4 ModelPos = { 0,0,0,1 };
		operator const std::string() const { return "ModelPos:" + (std::string)ModelPos; }
	};
	struct VaryingBase
	{
		Vector4 ClipPos = { 0,0,0,1 };
	};
	struct UniformBase
	{
		Matrix4x4 MVP;
		operator const std::string() const { return(std::string)MVP; }
	};
}
