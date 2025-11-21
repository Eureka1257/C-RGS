#include<string>
#include<memory>
#include<iostream>

#include"RGS/Application.h"
#include"RGS/Window.h"
#include"RGS/FrameBuffer.h"
#include"RGS/Renderer.h"
#include"RGS/Shaders/BlinnShader.h"
#include"RGS/Maths.h"

namespace RGS
{
	Application::Application(std::string name , const int width , const int height)
		: m_Name(name), m_Width(width), m_Height(height)
	{
		Init();
	}
	Application::~Application()
	{
		Terminate();
	}

	void Application::Init()
	{
		Window::Init();
		m_Window = Window::Create(m_Name, m_Width, m_Height);
	}
	void Application::Terminate()
	{
		delete m_Window;
		Window::Terminate();
	}
	void Application::Run()
	{
		//bool running = true;
		while (!m_Window->Closed())
		{
			OnUpdate();

			Window::PollInputEvents();
		}
	}
	void Application::OnUpdate()
	{
		//处理窗口接收到的事件
		if (m_Window->GetKey(RGS_KEY_0) == RGS_PRESS)
		{
			std::cout << "Key 0 Pressed" << std::endl;
		}
		if (m_Window->GetKey(RGS_KEY_Q) == RGS_PRESS)
		{
			std::cout << "Key Q Pressed" << std::endl;
		}
		if (m_Window->GetKey(RGS_KEY_W) == RGS_PRESS)
		{
			std::cout << "Key W Pressed" << std::endl;
		}
		if (m_Window->GetKey(RGS_KEY_E) == RGS_PRESS)
		{
			std::cout << "Key E Pressed" << std::endl;
		}
		if (m_Window->GetKey(RGS_KEY_R) == RGS_PRESS)
		{
			std::cout << "Key R Pressed" << std::endl;
		}
		
		FrameBuffer frameBuffer(m_Width, m_Height);
		frameBuffer.Clear({1.0f, 0.0f, 1.0f});
		m_Window->DrawFrameBuffer(frameBuffer);

		Program program(BlinnVertexShader);
		Triangle<BlinnVertex> tri;
		tri[0].ModelPos = { -5.0f, 5.0f, -5.0f, 1.0f };
		tri[1].ModelPos = { -5.0f, -5.0f, -5.0f, 1.0f };
		tri[2].ModelPos = {30.0f, -5.0f, -5.0f, 1.0f };

		BlinnUniforms uniforms;
		uniforms.MVP = Matrix4x4Perspective(90.0f / 180.0f * PI, 1.0f, 1.0f, 10.0f);

		Renderer::Draw(frameBuffer, program, tri, uniforms);
	}
}