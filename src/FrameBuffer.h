#pragma once
#include"Math.h"


namespace RGS 
{
	//Learn FrameBuffer: https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/05%20Framebuffers/
	class FrameBuffer
	{
	public:
		FrameBuffer(const int width, const int height);
		~FrameBuffer();

		/// <summary>
		/// 用于获取帧缓冲区的宽度
		/// </summary>
		/// <returns></returns>
		int GetWidth() const { return m_Width; }
		/// <summary>
		/// 用于获取帧缓冲区的高度
		/// </summary>
		/// <returns></returns>
		int GetHeight() const { return m_Height; }
		/// <summary>
		/// 设置某个像素点的颜色的函数
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="color"></param>
		void SetColor(const int x, const int y, const Vector3& color);
		/// <summary>
		/// 得到某个像素点的颜色的函数
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <returns></returns>
		Vector3 GetColor(const int x, const int y) const;
		/// <summary>
		/// 设置某个像素点的深度值的函数
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="depth"></param>
		void SetDepth(const int x, const int y, const float depth);
		/// <summary>
		/// 获取某个像素点的深度值的函数
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <returns></returns>
		float GetDepth(const int x, const int y) const;

		/// <summary>
		/// 用白色将画布清空的函数
		/// </summary>
		/// <param name="color"></param>
		void Clear(const Vector3& color = {0.0f, 0.0f, 0.0f});
		void ClearDepth(float depth = 1.0f);

	private:
		/// <summary>
		/// 获取某个像素点在数组中的索引值的函数
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <returns></returns>
		int GetPixelIndex(const int x, const int y) const { return y * m_Width + x; };

	private:
		int m_Width = 800;
		int m_Height = 600;
		int m_PixelSize;
		
		//1为最大值
		Vector3* m_ColorBuffer;
		float* m_DepthBuffer;
	};

}