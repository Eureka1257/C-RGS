#pragma once

#include"ShaderBase.h"
#include"RGS/Maths.h"

namespace RGS
{
	struct BlinnVertex : public VertexBase
	{
	};

	struct BlinnVaryings : public VaryingsBase
	{
	};

	struct BlinnUniforms : public UniformBase
	{
	};
	/// <summary>
	/// 将vertex中的数据经过uniform计算之后输出到varyings中
	/// </summary>
	/// <param name="varyings"></param>
	/// <param name="vertex"></param>
	/// <param name="uniforms"></param>
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);
	Vector4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);
}