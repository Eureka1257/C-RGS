#pragma once
#include"Base.h"
#include"FrameBuffer.h"

namespace RGS
{
	FrameBuffer::FrameBuffer(const int width, const int height)
		: m_Width(width), m_Height(height)
	{
		ASSERT((m_Width > 0) && (m_Height > 0), "宽度和高度必须大于0");
		m_PixelSize = m_Height * m_Width;
		//申请颜色缓冲区和深度缓冲区的内存
		m_ColorBuffer = new Vector3[m_PixelSize]();
		m_DepthBuffer = new float[m_PixelSize]();
		//默认清空
		Clear({0.0f, 0.0f, 0.0f});
		ClearDepth(1.0f);
	}

	FrameBuffer::~FrameBuffer()
	{
		delete[] m_ColorBuffer;
		delete[] m_DepthBuffer;

		m_ColorBuffer = nullptr;
		m_DepthBuffer = nullptr;
	}

	void FrameBuffer::SetColor(const int x, const int y, const Vector3& color)
	{		
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			m_ColorBuffer[index] = color;
		}
		else
		{
			ASSERT(false);
		}
		
	}

	Vector3 FrameBuffer::GetColor(const int x, const int y) const
	{		
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			return m_ColorBuffer[index];
		}
		else
		{
			ASSERT(false);
			return {0.0f, 0.0f, 0.0f};
		}
	}

	void FrameBuffer::SetDepth(const int x, const int y, const float depth)
	{
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			m_DepthBuffer[index] = depth;
		}
		else
		{
			//把宏取消定义了，这样方便看后面的报错
			ASSERT(false);
		}
	}
	float FrameBuffer::GetDepth(const int x, const int y) const
	{
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			return m_DepthBuffer[index];
		}
		else
		{
			ASSERT(false);
			return 0.0f;
		}
	}

	void FrameBuffer::Clear(const Vector3& color)
	{
		for (int i = 0; i < m_PixelSize; i++)
		{
			m_ColorBuffer[i] = color;
		}
	}
	void FrameBuffer::ClearDepth(float depth)
	{
		for (int i = 0; i < m_PixelSize; i++)
		{
			m_DepthBuffer[i] = depth;
		}
	}

}