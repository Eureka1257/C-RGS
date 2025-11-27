#pragma once
#include"RGS/Maths.h"
#include<string>

namespace RGS 
{
	
	struct VertexBase
	{
		Vector4 ModelPos = { 0.0f,0.0f,0.0f,1.0f };
		operator const std::string() const { return "ModelPos:" + (std::string)ModelPos; }
	};
	struct VaryingsBase
	{
		Vector4 ClipPos = { 0.0f,0.0f,0.0f,1.0f };
		Vector4 NdcPos = { 0.0f,0.0f,0.0f,1.0f };
		Vector4 FragPos = { 0.0f,0.0f,0.0f,1.0f };
	};
	struct UniformBase
	{
		Matrix4x4 MVP;
		operator const std::string() const { return(std::string)MVP; }
	};
}
