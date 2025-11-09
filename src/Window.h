#pragma once
#include <string>

#include "InputCodes.h"
#include "FrameBuffer.h"
namespace RGS 
{
	class Window 
	{
	public:
		Window(const std::string title, const int width, const int height);
		virtual ~Window() {};

		virtual void Show() const = 0;
		/// <summary>
		/// 用于绘制帧缓冲区的纯虚函数
		/// </summary>
		/// <param name="frameBuffer"></param>
		virtual void DrawFrameBuffer(const FrameBuffer& frameBuffer) = 0;
		/// <summary>
		/// 用于获取是否关闭窗口
		/// </summary>
		/// <returns></returns>
		bool Closed() const 
		{ 
			return m_Closed; 
		}
		/// <summary>
		/// 内联方法，用于读取用户输入
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		char GetKey(const uint32_t index) const 
		{
			return m_Keys[index];
		}
	public:
		/// <summary>
		/// 初始化一个窗口的函数，调用的是WindowsWindow的初始化
		/// </summary>
		static void Init();
		/// <summary>
		/// 结束一个窗口的函数，调用的是WindowsWindow的终止
		/// </summary>
		static void Terminate();
		/// <summary>
		/// 创建一个窗口的函数
		/// </summary>
		/// <param name="title">名字</param>
		/// <param name="width">宽度</param>
		/// <param name="height">长度</param>
		/// <returns></returns>
		static Window* Create(const std::string title , const int width , const int height);
		/// <summary>
		/// 分发输入事件的函数
		/// </summary>
		static void PollInputEvents();
	protected:
		std::string m_Title;
		int m_Width;
		int m_Height;
		bool m_Closed = false;

		char m_Keys[RGS_KEY_MAX_COUNT];
	};


}