#include"Math.h"

namespace RGS
{
	/// <summary>
	/// 将float转换成UChar类型的函数
	/// </summary>
	/// <param name="f"></param>
	/// <returns></returns>
	unsigned char Float2UChar(const float f)
	{
		return (unsigned char)(f * 255.0f);
	}
	/// <summary>
	/// 将UChar类型转换成float的函数
	/// </summary>
	/// <param name="c"></param>
	/// <returns></returns>
	float UChar2Float(const unsigned char c)
	{
		return (float)c / 255.0f;
	}
}