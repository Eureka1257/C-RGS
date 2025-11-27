#include"BlinnShader.h"

namespace RGS
{
	/// <summary>
	/// 将Model数据交给Uniforms处理之后传到varyings里面
	/// </summary>
	/// <param name="varyings"></param>
	/// <param name="vertex"></param>
	/// <param name="uniforms"></param>
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms)
	{
		varyings.ClipPos = uniforms.MVP * vertex.ModelPos;
	}
	Vector4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms)
	{
		discard = false;
		return{ varyings.NdcPos.X / 2 + 0.5f, varyings.NdcPos.Y / 2 + 0.5f, 0.0f, 1.0f };
	}
}