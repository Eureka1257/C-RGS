#pragma once

namespace RGS 
{
	struct Vector3
	{
		float X;
		float Y;
		float Z;

		//learn constexpr: https://learn.microsoft.com/zh-cn/cpp/cpp/constexpr-cpp?view=msvc-170
		//这个关键字的作用是告诉编译器用硬编码的方式写入代码中 ， 
		// 示例：constexpr Vector3 v（1.0f, 2.0f, 3.0f）;float x = v.X; 编译器会直接把1.0f写入x中，而不是在运行时创建一个Vector3对象再读取X成员
		// 这样可以提高性能，减少运行时的开销
		constexpr Vector3() 
			: X(0.0f), Y(0.0f), Z(0.0f) {}
		constexpr Vector3(float x, float y, float z)
			: X(x), Y(y), Z(z) {}
	};


	/// <summary>
	/// 将浮点数转换为 unsigned char 值。
	/// </summary>
	/// <param name="f">要转换的浮点数。</param>
	/// <returns>转换得到的 unsigned char 值。转换遵循 C/C++ 的浮点到整数转换规则，可能发生截断或溢出。</returns>
	unsigned char Float2UChar(const float f);
	/// <summary>
	/// 将 unsigned char 值转换为浮点数。
	/// </summary>
	/// <param name="c"></param>
	/// <returns></returns>
	float UChar2Float(const unsigned char c);
}