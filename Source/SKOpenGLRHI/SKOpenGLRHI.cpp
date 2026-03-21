#include "SKOpenGLRHI/SKOpenGLRHI.h"

#include <algorithm>
#include <cstring>
#include <vector>

#if defined(_WIN32)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <GL/gl.h>
#endif

namespace Skylark
{
	class FSKOpenGLTexture2D final : public ISKRHITexture2D
	{
	public:
		explicit FSKOpenGLTexture2D(const FSKRHITextureDesc& InDesc)
			: Desc(InDesc)
		{
			const uint32 W = std::max(1u, Desc.Width);
			const uint32 H = std::max(1u, Desc.Height);
			CpuPixels.resize(static_cast<SIZE_T>(W) * static_cast<SIZE_T>(H) * 4u, 0u);
		}

		const FSKRHITextureDesc& GetDesc() const override { return Desc; }

		uint8* GetPixelPtr(uint32 X, uint32 Y)
		{
			const uint32 W = std::max(1u, Desc.Width);
			const uint32 H = std::max(1u, Desc.Height);
			X = std::min(X, W - 1u);
			Y = std::min(Y, H - 1u);
			return CpuPixels.data() + ((static_cast<SIZE_T>(Y) * static_cast<SIZE_T>(W) + static_cast<SIZE_T>(X)) * 4u);
		}

	private:
		FSKRHITextureDesc Desc{};
		std::vector<uint8> CpuPixels;

		friend class FSKOpenGLCommandList;
		friend class FSKOpenGLRHIDevice;
	};

	class FSKOpenGLRHIDevice;

	class FSKOpenGLSwapChain final : public ISKRHISwapChain
	{
	public:
		FSKOpenGLSwapChain(FSKOpenGLRHIDevice* InOwner, const FSKRHISwapChainDesc& InDesc)
			: Owner(InOwner)
			, Desc(InDesc)
		{
		}

		~FSKOpenGLSwapChain() override
		{
			DestroyNative();
		}

		const FSKRHISwapChainDesc& GetDesc() const override { return Desc; }

		void Resize(uint32 NewWidth, uint32 NewHeight) override
		{
			Desc.Width = (NewWidth == 0) ? 1u : NewWidth;
			Desc.Height = (NewHeight == 0) ? 1u : NewHeight;
#if defined(_WIN32)
			if (MakeCurrent())
			{
				glViewport(0, 0, static_cast<GLsizei>(Desc.Width), static_cast<GLsizei>(Desc.Height));
			}
#endif
		}

		void Present() override
		{
#if defined(_WIN32)
			if (Hdc)
			{
				::SwapBuffers(Hdc);
			}
#endif
		}

		bool MakeCurrent()
		{
#if defined(_WIN32)
			if (!EnsureNative())
			{
				return false;
			}
			return ::wglMakeCurrent(Hdc, Glrc) == TRUE;
#else
			return false;
#endif
		}

	private:
		bool EnsureNative()
		{
#if defined(_WIN32)
			if (Hdc && Glrc)
			{
				return true;
			}

			HWND Hwnd = static_cast<HWND>(Desc.Window.Handle);
			if (!Hwnd)
			{
				return false;
			}

			Hdc = ::GetDC(Hwnd);
			if (!Hdc)
			{
				return false;
			}

			PIXELFORMATDESCRIPTOR Pfd{};
			Pfd.nSize = sizeof(Pfd);
			Pfd.nVersion = 1;
			Pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			Pfd.iPixelType = PFD_TYPE_RGBA;
			Pfd.cColorBits = 32;
			Pfd.cDepthBits = 24;
			Pfd.cStencilBits = 8;
			Pfd.iLayerType = PFD_MAIN_PLANE;

			const int Format = ::ChoosePixelFormat(Hdc, &Pfd);
			if (Format == 0)
			{
				DestroyNative();
				return false;
			}

			::SetPixelFormat(Hdc, Format, &Pfd);
			Glrc = ::wglCreateContext(Hdc);
			if (!Glrc)
			{
				DestroyNative();
				return false;
			}

			if (::wglMakeCurrent(Hdc, Glrc) != TRUE)
			{
				DestroyNative();
				return false;
			}

			glViewport(0, 0, static_cast<GLsizei>(Desc.Width), static_cast<GLsizei>(Desc.Height));
			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			return true;
#else
			return false;
#endif
		}

		void DestroyNative()
		{
#if defined(_WIN32)
			if (Hdc)
			{
				::wglMakeCurrent(nullptr, nullptr);
			}
			if (Glrc)
			{
				::wglDeleteContext(Glrc);
				Glrc = nullptr;
			}
			if (Hdc)
			{
				HWND Hwnd = static_cast<HWND>(Desc.Window.Handle);
				if (Hwnd)
				{
					::ReleaseDC(Hwnd, Hdc);
				}
				Hdc = nullptr;
			}
#endif
		}

	private:
		FSKOpenGLRHIDevice* Owner = nullptr;
		FSKRHISwapChainDesc Desc{};
#if defined(_WIN32)
		HDC Hdc = nullptr;
		HGLRC Glrc = nullptr;
#endif
	};

	class FSKOpenGLCommandList final : public ISKRHICommandList
	{
	public:
		explicit FSKOpenGLCommandList(FSKOpenGLRHIDevice* InOwner)
			: Owner(InOwner)
		{
		}

		void SetSwapChainRenderTarget(ISKRHISwapChain& SwapChain) override
		{
			BoundSwapChain = dynamic_cast<FSKOpenGLSwapChain*>(&SwapChain);
			BoundTexture = nullptr;
#if defined(_WIN32)
			if (BoundSwapChain)
			{
				BoundSwapChain->MakeCurrent();
			}
#endif
		}

		void SetRenderTargetTexture(ISKRHITexture2D& ColorTarget) override
		{
			BoundTexture = dynamic_cast<FSKOpenGLTexture2D*>(&ColorTarget);
			BoundSwapChain = nullptr;
		}

		void ClearRenderTarget(const FSKRHIClearColor& Color) override
		{
			if (BoundTexture)
			{
				const uint32 W = std::max(1u, BoundTexture->Desc.Width);
				const uint32 H = std::max(1u, BoundTexture->Desc.Height);
				const uint8 R = static_cast<uint8>(std::clamp(Color.R, 0.0f, 1.0f) * 255.0f);
				const uint8 G = static_cast<uint8>(std::clamp(Color.G, 0.0f, 1.0f) * 255.0f);
				const uint8 B = static_cast<uint8>(std::clamp(Color.B, 0.0f, 1.0f) * 255.0f);
				const uint8 A = static_cast<uint8>(std::clamp(Color.A, 0.0f, 1.0f) * 255.0f);

				for (uint32 Y = 0; Y < H; ++Y)
				{
					for (uint32 X = 0; X < W; ++X)
					{
						uint8* P = BoundTexture->GetPixelPtr(X, Y);
						P[0] = R; P[1] = G; P[2] = B; P[3] = A;
					}
				}
				return;
			}

#if defined(_WIN32)
			if (BoundSwapChain && BoundSwapChain->MakeCurrent())
			{
				const auto& Desc = BoundSwapChain->GetDesc();
				glViewport(0, 0, static_cast<GLsizei>(Desc.Width), static_cast<GLsizei>(Desc.Height));
				glClearColor(Color.R, Color.G, Color.B, Color.A);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}
#endif
		}

		void DrawLineList(const FSKRHILineVertex* Vertices, uint32 VertexCount, const FSKRHILineDrawParams& Params) override
		{
			if (!Vertices || VertexCount < 2 || !BoundSwapChain)
			{
				return;
			}

#if defined(_WIN32)
			if (!BoundSwapChain->MakeCurrent())
			{
				return;
			}

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			if (Params.bDepthTest)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}

			glLineWidth((Params.Width <= 0.0f) ? 1.0f : Params.Width);
			glBegin(GL_LINES);
			for (uint32 Index = 0; Index < VertexCount; ++Index)
			{
				const FSKRHILineVertex& V = Vertices[Index];
				const uint8 R = static_cast<uint8>((V.ColorRGBA8 >> 0) & 0xFFu);
				const uint8 G = static_cast<uint8>((V.ColorRGBA8 >> 8) & 0xFFu);
				const uint8 B = static_cast<uint8>((V.ColorRGBA8 >> 16) & 0xFFu);
				const uint8 A = static_cast<uint8>((V.ColorRGBA8 >> 24) & 0xFFu);
				glColor4ub(R, G, B, A);
				glVertex4f(V.X, V.Y, V.Z, V.W);
			}
			glEnd();
#else
			(void)Params;
#endif
		}

		void Flush() override
		{
#if defined(_WIN32)
			if (BoundSwapChain)
			{
				::glFlush();
			}
#endif
		}

	private:
		FSKOpenGLRHIDevice* Owner = nullptr;
		FSKOpenGLSwapChain* BoundSwapChain = nullptr;
		FSKOpenGLTexture2D* BoundTexture = nullptr;
	};

	class FSKOpenGLRHIDevice final : public ISKRHIDevice
	{
	public:
		FSKOpenGLRHIDevice()
			: Immediate(this)
		{
		}

		bool Init(const FSKRHIDeviceDesc& InDesc) override
		{
			Desc = InDesc;
			return true;
		}

		void Shutdown() override {}
		void BeginFrame() override {}
		void EndFrame() override {}

		ISKRHICommandList& GetImmediateCommandList() override { return Immediate; }

		TUniquePtr<ISKRHISwapChain> CreateSwapChain(const FSKRHISwapChainDesc& Desc) override
		{
			return std::make_unique<FSKOpenGLSwapChain>(this, Desc);
		}

		TUniquePtr<ISKRHITexture2D> CreateTexture2D(const FSKRHITextureDesc& Desc) override
		{
			return std::make_unique<FSKOpenGLTexture2D>(Desc);
		}

		bool ReadbackTexturePixelRGBA8(ISKRHITexture2D& Texture, uint32 X, uint32 Y, uint8 OutRGBA[4]) override
		{
			auto* Tex = dynamic_cast<FSKOpenGLTexture2D*>(&Texture);
			if (!Tex || !OutRGBA)
			{
				return false;
			}

			uint8* P = Tex->GetPixelPtr(X, Y);
			std::memcpy(OutRGBA, P, 4u);
			return true;
		}

		ESKRHIApi GetApi() const override { return ESKRHIApi::OpenGL; }

	private:
		FSKRHIDeviceDesc Desc{};
		FSKOpenGLCommandList Immediate;
	};

	TUniquePtr<ISKRHIDevice> SKCreateOpenGLRHIDevice()
	{
		return std::make_unique<FSKOpenGLRHIDevice>();
	}
}
