#pragma once
#include<string>

namespace RGS 
{
	struct Vector2 
	{
		float X, Y;

		constexpr Vector2() 
			: X(0.0f), Y(0.0f) {}
		constexpr Vector2(float x, float y) 
			: X(x), Y(y) {}
	};
	struct Vector3
	{
		float X;
		float Y;
		float Z;

		
		//这个关键字的作用是告诉编译器用硬编码的方式写入代码中 ， 
		// 示例：constexpr Vector3 v（1.0f, 2.0f, 3.0f）;float x = v.X; 编译器会直接把1.0f写入x中，而不是在运行时创建一个Vector3对象再读取X成员
		// 这样可以提高性能，减少运行时的开销
		constexpr Vector3() 
			: X(0.0f), Y(0.0f), Z(0.0f) {}
		constexpr Vector3(float x, float y, float z)
			: X(x), Y(y), Z(z) {}
		/// <summary>
		/// 将三维向量强转成二维向量
		/// </summary>
		operator Vector2() const
		{
			return{ X, Y };
		}
		
	};
	struct Vector4 {
		float X;
		float Y;
		float Z;
		float W;
		constexpr Vector4()
			: X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
		constexpr Vector4(float x, float y, float z, float w)
			: X(x), Y(y), Z(z), W(w) {}
		constexpr Vector4(const Vector3& vec3, float w)
			: X(vec3.X), Y(vec3.Y), Z(vec3.Z), W(w) {
		}

		operator Vector2() const
		{
			return{ X, Y };
		}
		operator Vector3() const
		{
			return{ X, Y, Z };
		}

		operator const std::string()const
		{
			std::string res;
			res += "(";
			res += std::to_string(X) + ", ";
			res += std::to_string(Y) + ", ";
			res += std::to_string(Z) + ", ";
			res += std::to_string(W) + ")";
			return res;
		}
	};
	//这里按照行优先存储，第一列是m[0][0],m[1][0],m[2][0],m[3][0]
	struct Matrix4x4
	{
		float m[4][4];
		Matrix4x4()
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					m[i][j] = 0.0f;
				}
			}
		}
		/// <summary>
		/// 按照列向量初始化矩阵
		/// </summary>
		/// <param name="v0"></param>
		/// <param name="v1"></param>
		/// <param name="v2"></param>
		/// <param name="v3"></param>
		Matrix4x4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3)
		{
			m[0][0] = v0.X; m[1][0] = v0.Y; m[2][0] = v0.Z; m[3][0] = v0.W;
			m[0][1] = v1.X; m[1][1] = v1.Y; m[2][1] = v1.Z; m[3][1] = v1.W;
			m[0][2] = v2.X; m[1][2] = v2.Y; m[2][2] = v2.Z; m[3][2] = v2.W;
			m[0][3] = v3.X; m[1][3] = v3.Y; m[2][3] = v3.Z; m[3][3] = v3.W;
		};

		operator const std::string() const
		{
			std::string res;
			res += "(";
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					res += std::to_string(m[i][j]);
					res += (i == 3 && j == 3) ? ")" : ", ";
				}
			}
			return res;
		}
	};

	float Dot(const Vector3& left, const Vector3& right);
	/// <summary>
	/// 四维向量与矩阵的乘法
	/// </summary>
	/// <param name="mat"></param>
	/// <param name="vec"></param>
	/// <returns></returns>
	Vector4 operator*(const Matrix4x4& mat, const Vector4& vec);
	/// <summary>
	/// 矩阵之间的乘法
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	Matrix4x4 operator*(const Matrix4x4& left, const Matrix4x4& right);	
	/// <summary>
	/// 矩阵的*=运算符重载
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <returns></returns>
	Matrix4x4& operator*=(Matrix4x4& left, const Matrix4x4& right);

	/// <summary>
	/// 单位矩阵
	/// </summary>
	/// <returns></returns>
	Matrix4x4 Matrix4x4Identity();
	/// <summary>
	/// 缩放矩阵
	/// </summary>
	/// <param name="sx"></param>
	/// <param name="sy"></param>
	/// <param name="sz"></param>
	/// <returns></returns>
	Matrix4x4 Matrix4x4Scale(float sx, float sy, float sz);
	/// <summary>
	/// 平移矩阵
	/// </summary>
	/// <param name="tx"></param>
	/// <param name="ty"></param>
	/// <param name="tz"></param>
	/// <returns></returns>
	Matrix4x4 Matrix4x4Translate(float tx, float ty, float tz);
	/// <summary>
	/// 绕X轴旋转的矩阵
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	Matrix4x4 Matrix4x4RotateX(float angle);
	/// <summary>
	/// 绕Y轴旋转的矩阵
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	Matrix4x4 Matrix4x4RotateY(float angle);
	/// <summary>
	/// 绕Z轴旋转的矩阵
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	Matrix4x4 Matrix4x4RotateZ(float angle);
	/// <summary>
	/// 把摄像机旋转并移动到世界原点的矩阵
	/// </summary>
	/// <param name="xAxis"></param>
	/// <param name="yAxis"></param>
	/// <param name="zAxis"></param>
	/// <param name="eye"></param>
	/// <returns></returns>
	Matrix4x4 Natrix4x4LookAt(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3& eye);
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