#pragma once
#include"windows.h"

namespace RGS 
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const std::string title, const int width, const int height);
		~WindowsWindow();
		/// <summary>
		/// 把在内存区绘制好的内容拷贝到屏幕的函数
		/// </summary>
		virtual void Show() const override;	
		/// <summary>
		/// 用于绘制帧缓冲区的函数
		/// </summary>
		/// <param name="frameBuffer"></param>
		void DrawFrameBuffer(const FrameBuffer& frameBuffer) override;
	public:
		/// <summary>
		/// 初始化的函数
		/// </summary>
		static void Init();
		/// <summary>
		/// 执行终止操作的静态函数。
		/// </summary>
		static void Terminate();
		/// <summary>
		/// 分发输入事件的函数
		/// </summary>
		static void PollInputEvents();

	private:
		/// <summary>
		/// 注册窗口的函数
		/// </summary>
		static void Register();
		/// <summary>
		/// 注销窗口的函数
		/// </summary>
		static void Unregister();
		/// <summary>
		/// 处理指定窗口的按键事件的内部实现函数。
		/// </summary>
		/// <param name="window">处理事件的窗口</param>
		/// <param name="keycode">键盘的键值</param>
		/// <param name="state">状态（按下抬起）</param>
		static void KeyPressImpl(WindowsWindow* window, const WPARAM keycode, const char state);
		/// <summary>
		/// 窗口过程处理函数 ，Windows规定除了函数名以外都必须这么写
		/// </summary>
		/// <param name="hwnd"></param>
		/// <param name="msgID"></param>
		/// <param name="wParam"></param>
		/// <param name="lParam"></param>
		/// <returns></returns>		
		static LRESULT CALLBACK WndProc(const HWND hwnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam);


	private:
		HWND m_Handle;
		HDC m_MemoryDC;
		//以后在这个指针指向的进行写入，也就是在这画画
		unsigned char* m_Buffer;

		static bool s_Inited;
	};
}