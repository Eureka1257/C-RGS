#pragma once
#include"windows.h"

namespace RGS 
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const std::string title, const int width, const int height);
		~WindowsWindow();

		virtual void Show() const override;	

	public:
		static void Init();
		static void Terminate();

	private:
		static void Register();
		static void Unregister();
		//窗口处理函数 ，Windows规定除了函数名以外都必须这么写
		static LRESULT CALLBACK WndProc(const HWND hwnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam);

	private:
		HWND m_Handle;
		HDC m_MemoryDC;
		//以后在这个指针指向的进行写入，也就是在这画画
		unsigned char* m_Buffer;

		static bool s_Inited;
	};
}