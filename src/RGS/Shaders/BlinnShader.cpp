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
}