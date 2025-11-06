#include"Base.h"
#include"Window.h"
#include"WindowsWindow.h"

#include<windows.h>

#define RGS_WINDOW_ENTRY_NAME "Entry"
#define RGS_WINDOW_CLASS_NAME "Class"

namespace RGS
{
	bool WindowsWindow::s_Inited = false;
	void WindowsWindow::Init()
	{
		//调用注册方法
		ASSERT(!s_Inited);
		Register();
		s_Inited = true;
	}
	void WindowsWindow::Terminate()
	{
		//调用注销方法
		ASSERT(!s_Inited);
		Unregister();
		s_Inited = false;
	}
	//注册窗口
	void WindowsWindow::Register()
	{
		ATOM atom;
		WNDCLASS wc = { 0 };
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		//背景色
		wc.hbrBackground = (HBRUSH)(WHITE_BRUSH);
		//默认光标
		wc.hCursor = NULL;
		//默认图标
		wc.hIcon = NULL;
		wc.hInstance = GetModuleHandle(NULL);
		//窗口处理函数
		wc.lpfnWndProc = WindowsWindow::WndProc;
		wc.lpszClassName = RGS_WINDOW_CLASS_NAME;
		//拉伸时重绘
		wc.style = CS_HREDRAW | CS_VREDRAW;
		//不要菜单
		wc.lpszMenuName = NULL;
		//注册窗口
		atom = RegisterClass(&wc);
	}
	//注销窗口
	void WindowsWindow::Unregister()
	{
		UnregisterClass(RGS_WINDOW_CLASS_NAME, GetModuleHandle(NULL));
	}
	LRESULT CALLBACK WindowsWindow::WndProc(const HWND hwnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam)
	{
		WindowsWindow* window = (WindowsWindow*)GetProp(hwnd, RGS_WINDOW_ENTRY_NAME);
		if (window == nullptr)
		{
			//默认的窗口处理函数
			return DefWindowProc(hwnd, msgID, wParam, lParam);
		}

		switch (msgID)
		{
		case WM_DESTROY:
			window->m_Closed = true;
			return 0;			
		}
		return DefWindowProc(hwnd, msgID, wParam, lParam);
	}
	
	void WindowsWindow::Show() const
	{
		HDC windowDC = GetDC(m_Handle);
		//将内存中画好的内容拷贝到屏幕上
		BitBlt(windowDC, 0, 0, m_Width,m_Height, m_MemoryDC, 0, 0, SRCCOPY);
		ShowWindow(m_Handle, SW_SHOW);
		ReleaseDC(m_Handle, windowDC);
	}
	WindowsWindow::~WindowsWindow()
	{
		//先把窗口藏起来
		ShowWindow(m_Handle, SW_HIDE);
		//删除绑定了this的键值对
		RemoveProp(m_Handle, RGS_WINDOW_ENTRY_NAME);
		//删除内存设备上下文
		DeleteDC(m_MemoryDC);
		//销毁窗口
		DestroyWindow(m_Handle);

	}

	//怎么创建窗口
	WindowsWindow::WindowsWindow(const std::string title, const int width, const int height) :Window(title, width, height)
	{
		ASSERT((s_Inited), "未初始化");
		//窗口样式
		DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.bottom = (long)height;
		rect.right = (long)width;
		AdjustWindowRect(&rect, style, false);
		//参数一一定要填之前创建的窗口类的名字
		//m_Handle是Windows窗口的句柄，用来标识窗口的唯一标记
		m_Handle = CreateWindow(RGS_WINDOW_CLASS_NAME, m_Title.c_str(), style,
			CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top,
			NULL, NULL, GetModuleHandle(NULL), NULL);
		//看条件是否生效
		ASSERT(m_Handle != nullptr);
		//生效的话说明已经成功打开窗口
		m_Closed = false;

		//给窗口粘贴一些附加的属性
		SetProp(m_Handle, RGS_WINDOW_ENTRY_NAME, this);

		//先从内存中获取一块区域，再往这片内存空间中填内容，而不是直接把内容显示到窗口上
		//这个功能叫内存设备上下文，就是DC
		HDC windowDC = GetDC(m_Handle);
		m_MemoryDC = CreateCompatibleDC(windowDC);

		BITMAPINFOHEADER bitHeader = {};
		HBITMAP newBitmap;
		HBITMAP oldBitmap;

		bitHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitHeader.biWidth =((long)m_Width);
		bitHeader.biHeight = -((long)m_Height);
		//指定目标设备的平面数，必须设置为1
		bitHeader.biPlanes = 1;
		//每个像素需要设置几个位，这里的意思就是每个颜色八个位
		bitHeader.biBitCount = 24;
		bitHeader.biCompression = BI_RGB;

		//分配空间 学习网站：https://learn.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-createdibsection
		//创建位图
		newBitmap = CreateDIBSection(m_MemoryDC, (BITMAPINFO*)&bitHeader, DIB_RGB_COLORS, (void**)&m_Buffer, nullptr , 0);
		ASSERT(newBitmap != nullptr);
		constexpr int channelCount = 3;
		int size = m_Width * m_Height & channelCount * sizeof(unsigned char);
		//先全部设置成黑色
		memset(m_Buffer, 0, size);
		oldBitmap = (HBITMAP)SelectObject(m_MemoryDC, newBitmap);

		DeleteObject(oldBitmap);
		ReleaseDC(m_Handle, windowDC);

		Show();
	}
}