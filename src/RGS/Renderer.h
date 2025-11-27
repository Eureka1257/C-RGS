#pragma once

#include"RGS/Maths.h"
#include"RGS/FrameBuffer.h"
#include"RGS/Base.h"
#include<type_traits>
#include<cmath>

namespace RGS
{
	template<typename vertex_t>
	struct Triangle
	{
		static_assert(std::is_base_of_v<VertexBase, vertex_t>, "vertex_t 必须继承自RGS::VertexBase");

		vertex_t Vertex[3];

		vertex_t& operator[](size_t i) { return Vertex[i]; }
		const vertex_t& operator[](size_t i) const { return Vertex[i]; }

		Triangle() = default;
	};
	template<typename vertex_t, typename uniforms_t, typename varyings_t>
	//用于保存单次渲染所需的参数
	struct Program
	{
		using vertex_shader_t = void(*)(varyings_t&, const vertex_t&, const uniforms_t&);
		vertex_shader_t VertexShader;

		using fragment_shader_t = Vector4(*)(bool& discard, const varyings_t&, const uniforms_t&);
		fragment_shader_t FragmentShader;
		Program(const vertex_shader_t vertexShader, const fragment_shader_t fragmentShader)
			:VertexShader(vertexShader), FragmentShader(fragmentShader) {}
	};

	class Renderer
	{
	private:
		static constexpr int RGS_MAX_VARYINGS = 9;

	private:
		enum class Plane
		{
			POSITIVE_W,
			POSITIVE_X,
			NEGATIVE_X,
			POSITIVE_Y,
			NEGATIVE_Y,
			POSITIVE_Z,
			NEGATIVE_Z,

		};

		struct BoundingBox { int MinX, MaxX, MinY, MaxY; };

		static BoundingBox GetBoundingBox(const Vector4(&fragCoords)[3],
			const int width,
			const int height);

		static void CaculateWeights(float(&screenWeights)[3],
			float(&weights)[3],
			const Vector4(&fragCoords)[3],
			const Vector2& screenPoint);

		static bool IsVertexVisible(const Vector4& clipPos);
		
		static bool IsInsidePlane(const Vector4& clipPos, const Plane plane);

		static float GetIntersectRatio(const Vector4& prev, const Vector4 curr, const Plane plane);

		static bool IsInsideTriangle(float(&weights)[3]);
		
		

		template<typename varyings_t>
		static int Clip(varyings_t(&varyings)[RGS_MAX_VARYINGS])
		{
			//判断顶点可见性
			bool v0_Visible = IsVertexVisible(varyings[0].ClipPos);
			bool v1_Visible = IsVertexVisible(varyings[1].ClipPos);
			bool v2_Visible = IsVertexVisible(varyings[2].ClipPos);
			if (v0_Visible && v1_Visible && v2_Visible)
			{
				ASSERT(v0_Visible && v1_Visible && v2_Visible);
				return 3;
			}

			int vertexNum = 3;
			varyings_t varyings_[RGS_MAX_VARYINGS];
			vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::POSITIVE_W, vertexNum);
			if (vertexNum == 0) { return 0; }
			vertexNum = ClipAgainstPlane(varyings, varyings_, Plane::POSITIVE_X, vertexNum);
			if (vertexNum == 0) { return 0; }
			vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::NEGATIVE_X, vertexNum);
			if (vertexNum == 0) { return 0; }
			vertexNum = ClipAgainstPlane(varyings, varyings_, Plane::POSITIVE_Y, vertexNum);
			if (vertexNum == 0) { return 0; }
			vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::NEGATIVE_Y, vertexNum);
			if (vertexNum == 0) { return 0; }
			vertexNum = ClipAgainstPlane(varyings, varyings_, Plane::POSITIVE_Z, vertexNum);
			if (vertexNum == 0) { return 0; }
			vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::NEGATIVE_Z, vertexNum);
			if (vertexNum == 0) { return 0; }

			memcpy(varyings, varyings_, sizeof(varyings_));

			return vertexNum;

		}

		template<typename varyings_t>
		static void CaculateNdcPos(varyings_t(&varyings)[RGS_MAX_VARYINGS], const int vertexNum)
		{
			for (int i = 0; i < vertexNum; i++)
			{
				float w = varyings[i].ClipPos.W;
				varyings[i].NdcPos = varyings[i].ClipPos / w;
				varyings[i].NdcPos.W = 1.0f / w;
			}
		}

		/// <summary>
		/// 计算视口变换之后的顶点位置的方法
		/// </summary>
		/// <typeparam name="varyings_t"></typeparam>
		/// <param name="varyings"></param>
		/// <param name="vertexNum"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		template<typename varyings_t>
		static void CaculateFragPos(varyings_t(&varyings)[RGS_MAX_VARYINGS],
			const int vertexNum,
			const float width,
			const float height)
		{
			for (int i = 0; i < vertexNum; i++)
			{
				float x = ((varyings[i].NdcPos.X + 1.0f) * 0.5f * width);
				float y = ((varyings[i].NdcPos.Y + 1.0f) * 0.5f * height);
				//此时z值只需要保证正确的前后关系即可
				float z = ((varyings[i].NdcPos.Z + 1.0f) * 0.5f);
				float w = varyings[i].NdcPos.W;

				varyings[i].FragPos.X = x;
				varyings[i].FragPos.Y = y;
				varyings[i].FragPos.Z = z;
				varyings[i].FragPos.W = w;
			}
		}
		/// <summary>
		/// 根据点的属性调用片元着色器计算颜色结果
		/// </summary>
		/// <typeparam name="vertex_t"></typeparam>
		/// <typeparam name="uniforms_t"></typeparam>
		/// <typeparam name="varyings_t"></typeparam>
		/// <param name="frameBuffer"></param>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="program"></param>
		/// <param name="varyings"></param>
		/// <param name="uniforms"></param>
		template<typename vertex_t, typename uniforms_t, typename varyings_t>
		static void ProcessPixel(FrameBuffer& frameBuffer,
			const int x,
			const int y,
			const Program<vertex_t, uniforms_t, varyings_t>& program,
			const varyings_t varyings,
			const uniforms_t& uniforms)
		{
			//Pixel Shading
			//是否丢弃像素
			bool discard = false;
			Vector4 color{ 0.0f, 0.0f, 0.0f, 0.0f };
			color = program.FragmentShader(discard, varyings, uniforms);
			if (discard)
			{
				return;
			}
			color.X = Clamp(color.X, 0.0f, 1.0f);
			color.Y = Clamp(color.Y, 0.0f, 1.0f);
			color.Z = Clamp(color.Z, 0.0f, 1.0f);
			color.W = Clamp(color.W, 0.0f, 1.0f);
			//将颜色存储到帧缓存上
			frameBuffer.SetColor(x, y, color);
		}

		template<typename varyings_t>
		static void LerpVaryings(varyings_t& out, const varyings_t& start, const varyings_t& end, const float ratio)
		{
			constexpr uint32_t floatNum = sizeof(varyings_t) / sizeof(float);
			float* startFloat = (float*)&start;
			float* endFloat = (float*)&end;
			float* outFloat = (float*)&out;

			for (int i = 0; i < (int)floatNum; i++)
			{
				outFloat[i] = Lerp(startFloat[i], endFloat[i], ratio);
			}

		}

		/// <summary>
		/// 用来插值已完成裁剪的三角形的内部的各种变量的函数
		/// </summary>
		/// <typeparam name="varyings_t"></typeparam>
		/// <param name="out"></param>
		/// <param name="varyings"></param>
		/// <param name="weights"></param>
		template<typename varyings_t>
		static void LerpVaryings(varyings_t& out, const varyings_t(&varyings)[3], float(&weights)[3])
		{
			constexpr uint32_t floatNum = sizeof(varyings_t) / sizeof(float);
			//三个指针指向三角形的三个顶点
			float* v0 = (float*)&varyings[0];
			float* v1 = (float*)&varyings[1];
			float* v2 = (float*)&varyings[2];
			float* outFloat = (float*)&out;
			//把三角形内的点全都乘上对应的权重
			for (int i = 0; i < (int)floatNum; i++)
			{
				outFloat[i] = v0[i] * weights[0] + v1[i] * weights[1] + v2[i] * weights[2];
			}
		}

		template<typename varyings_t>
		static int ClipAgainstPlane(varyings_t(&outVaryings)[RGS_MAX_VARYINGS],
			const varyings_t(&inVaryings)[RGS_MAX_VARYINGS],
			const Plane plane,
			const int inVertexNum)
		{
			ASSERT(inVertexNum >= 3);

			int outVertexNum = 0;
			for (int i = 0; i < inVertexNum; i++)
			{
				//当前点与上一个点的索引
				int prevIndex = (i - 1 + inVertexNum) % inVertexNum;
				int currIndex = i;

				const varyings_t& prevVaryings = inVaryings[prevIndex];
				const varyings_t& currVaryings = inVaryings[currIndex];

				const bool prevInside = IsInsidePlane(prevVaryings.ClipPos, plane);
				const bool currInside = IsInsidePlane(currVaryings.ClipPos, plane);

				if (currInside != prevInside)
				{
					//把交点加到数组中
					float ratio = GetIntersectRatio(prevVaryings.ClipPos, currVaryings.ClipPos, plane);
					LerpVaryings(outVaryings[outVertexNum], prevVaryings, currVaryings, ratio);
					outVertexNum++;
				}

				if (currInside)
				{
					//如果当前的点在视锥体内就直接把它加到数组中
					outVaryings[outVertexNum] = inVaryings[currIndex];
					outVertexNum++;
				}
			}
			ASSERT(outVertexNum <= RGS_MAX_VARYINGS);
			return outVertexNum;
		}
	public:
		/// <summary>
		/// 齐次除法
		/// </summary>
		/// <typeparam name="varyings_t"></typeparam>
		/// <param name="varyings"></param>
		/// <param name="vertexNum"></param>
		

		template<typename vertex_t, typename uniforms_t, typename varyings_t>
		static void RasterizeTriangle(FrameBuffer& frameBuffer,
			const Program<vertex_t, uniforms_t, varyings_t>& program,
			const varyings_t(&varyings)[3],
			const uniforms_t& uniforms)
		{
			//Bounding Box set up
			Vector4 fragCoords[3];
			fragCoords[0] = varyings[0].FragPos;
			fragCoords[1] = varyings[1].FragPos;
			fragCoords[2] = varyings[2].FragPos;
			BoundingBox bBox = GetBoundingBox(fragCoords, frameBuffer.GetWidth(), frameBuffer.GetHeight());
			
			for (int y = bBox.MinY; y < bBox.MaxY; y++)
			{
				for (int x = bBox.MinX; x < bBox.MaxX; x++)
				{
					//Varyings set up
					float screenWeights[3];
					float weights[3];
					Vector2 screenPoint{ (float)x + 0.5f, (float)y + 0.5f };

					CaculateWeights(screenWeights, weights, fragCoords, screenPoint);
					if (!IsInsideTriangle(weights))
					{
						continue;
					}

					varyings_t pixVaryings;
					LerpVaryings(pixVaryings, varyings, weights);

					//Pixel processing
					ProcessPixel(frameBuffer, x, y, program, pixVaryings, uniforms);
				}
			}
		}
			//画画的函数
		template<typename vertex_t, typename uniforms_t, typename varyings_t>
		static void Draw(FrameBuffer& frameBuffer,
			const Program<vertex_t, uniforms_t, varyings_t>& program,
			const Triangle<vertex_t>& triangle,
			const uniforms_t& uniforms)
			{
				static_assert(std::is_base_of_v<VertexBase, vertex_t>, "vertex_t必须继承自 RGS::VertexBase");
				static_assert(std::is_base_of_v<VaryingsBase, varyings_t>, "vertex_t必须继承自 RGS::VaryingsBase");

				//Vertex Shading & Projection
				varyings_t varyings[RGS_MAX_VARYINGS];
				for (int i = 0; i < 3; i++)
				{
					program.VertexShader(varyings[i], triangle[i], uniforms);
				}
				//Cliping
				int vertexNum = Clip(varyings);

				//ScreenMapping
				CaculateNdcPos(varyings, vertexNum);
				int fWidth = frameBuffer.GetWidth();
				int fHeight = frameBuffer.GetHeight();
				CaculateFragPos(varyings, vertexNum, (float)fWidth, (float)fHeight);

				//Triangle Assembly & Rasterization
				for (int i = 0; i < vertexNum - 2; i++)
				{
					varyings_t triVaryings[3];
					triVaryings[0] = varyings[0];
					triVaryings[1] = varyings[i + 1];
					triVaryings[2] = varyings[i + 2];

					RasterizeTriangle(frameBuffer, program, triVaryings, uniforms);
				}
			}
		
	};
}