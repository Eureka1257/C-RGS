#include"Math.h"
#include<string>
#include"Base.h"

namespace RGS
{
	float Dot(const Vector3& left, const Vector3& right)
	{
		return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
	}
	Vector3 Cross(const Vector3& left, const Vector3& right)
	{
		return Vector3(
			left.Y * right.Z - left.Z * right.Y,
			left.Z * right.X - left.X * right.Z,
			left.X * right.Y - left.Y * right.X
		);
	}	
	Vector3 Normalize(Vector3 &v)
	{
		float len = (float)sqrt(Dot(v, v));
		return v / len;
	}
	/*Matrix4x4::Matrix4x4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3)
	{
		
	}*/
	Vector3 operator+(const Vector3& left, const Vector3& right)
	{
		return { left.X + right.X, left.Y + right.Y, left.Z + right.Z };
	}
	Vector3 operator-(const Vector3& left, const Vector3& right)
	{
		return left + (-1.0f * right);
	}
	
	Vector3 operator/(const Vector3& left, const float right)
	{
		return left * (1.0f / right);
	}
	Vector3 operator*(const Vector3& left, const float right)
	{
		return right * left;
	}
	Vector3 operator*(const float left, const Vector3& right)
	{
		return{left * right.X, left * right.Y, left * right.Z};
	}

	Vector4 operator*(const Matrix4x4& mat, const Vector4& vec)
	{
		Vector4 res;
		res.X = mat.m[0][0] * vec.X + mat.m[0][1] * vec.Y + mat.m[0][2] * vec.Z + mat.m[0][3] * vec.W;
		res.Y = mat.m[1][0] * vec.X + mat.m[1][1] * vec.Y + mat.m[1][2] * vec.Z + mat.m[1][3] * vec.W;
		res.Z = mat.m[2][0] * vec.X + mat.m[2][1] * vec.Y + mat.m[2][2] * vec.Z + mat.m[2][3] * vec.W;
		res.W = mat.m[3][0] * vec.X + mat.m[3][1] * vec.Y + mat.m[3][2] * vec.Z + mat.m[3][3] * vec.W;
		return {};
	}
	Matrix4x4 operator*(const Matrix4x4& left, const Matrix4x4& right)
	{
		Matrix4x4 res;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					res.m[i][j] += left.m[i][k] * right.m[k][j];
				}				
			}
		}
		return res;
	}
	Matrix4x4& operator*=(Matrix4x4& left, const Matrix4x4& right)
	{
		left = left * right;
		return left;
	}
	

	Matrix4x4 Matrix4x4Identity()
	{
		return Matrix4x4({ 1,0,0,0 }, { 0,1,0,0 }, { 0,0,1,0 }, {0,0,0,1});
	}
	Matrix4x4 Matrix4x4Scale(const float sx, const float sy, const float sz)
	{
		Matrix4x4 m = Matrix4x4Identity();
		ASSERT(sx != 0 && sy != 0 && sz != 0);
		return Matrix4x4({ sx,0,0,0 }, { 0,sy,0,0 }, { 0,0,sz,0 }, { 0,0,0,1 });
	}
	Matrix4x4 Matrix4x4Translate(const float tx, const float ty, const float tz)
	{
		Matrix4x4 m = Matrix4x4Identity();
		return Matrix4x4({ 1,0,0,tx }, { 0,1,0,ty }, { 0,0,1,tz }, { 0,0,0,1 });
	}
	Matrix4x4 Matrix4x4RotateX(const float angle)
	{
		float c = (float)cos(angle);
		float s = (float)sin(angle);

		return Matrix4x4({ 1,0,0,0 }, { 0,c, -s,0 }, { 0,s,c,0 }, { 0,0,0,1 });
	}
	Matrix4x4 Matrix4x4RotateY(const float angle)
	{
		float c = (float)cos(angle);
		float s = (float)sin(angle);

		return Matrix4x4({ c,0,s,0 }, { 0,1,0,0 }, { -s,0,c,0 }, { 0,0,0,1 });
	}
	Matrix4x4 Matrix4x4RotateZ(const float angle)
	{
		float c = (float)cos(angle);
		float s = (float)sin(angle);

		return Matrix4x4({ c, -s,0,0 }, { s,c,0,0 }, { 0,0,1,0 }, { 0,0,0,1 });
	}
	
	Matrix4x4 Matrix4x4LookAt(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3& eye)
	{
		Matrix4x4 m = Matrix4x4Identity();
		m.m[0][0] = xAxis.X;
		m.m[0][1] = xAxis.Y;
		m.m[0][2] = xAxis.Z;

		m.m[1][0] = yAxis.X;
		m.m[1][1] = yAxis.Y;
		m.m[1][2] = yAxis.Z;

		m.m[2][0] = zAxis.X;
		m.m[2][1] = zAxis.Y;
		m.m[2][2] = zAxis.Z;
		//将摄像机平移到世界原点
		m.m[0][3] = -Dot(xAxis, eye);
		m.m[1][3] = -Dot(yAxis, eye);
		m.m[2][3] = -Dot(zAxis, eye);

		return m;
	}
	Matrix4x4 Matrix4x4LookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
	{
		Vector3 zAxis = Normalize(eye - target);
		Vector3 xAxis = Normalize(Cross(up, zAxis));
		Vector3 yAxis = Normalize(Cross(zAxis, xAxis));

		return Matrix4x4LookAt(xAxis, yAxis, zAxis, eye);
	}
	Matrix4x4 Matrix4x4Perspective(float fovy, float aspect, float near, float far)
	{
		float z_range = far - near;
		Matrix4x4 m = Matrix4x4Identity();
		ASSERT(fovy > 0 && aspect > 0);
		ASSERT(near > 0 && far > 0 && z_range > 0);
		m.m[1][1] = 1 / (float)tan(fovy / 2);
		m.m[0][0] = m.m[1][1] / aspect;
		m.m[2][2] = -(near + far) / z_range;
		m.m[2][3] = -2 * far * near / z_range;
		m.m[3][2] = -1;
		m.m[3][3] = 0;

		return m;
	}
	
	unsigned char Float2UChar(const float f)
	{
		return (unsigned char)(f * 255.0f);
	}
	
	float UChar2Float(const unsigned char c)
	{
		return (float)c / 255.0f;
	}
}