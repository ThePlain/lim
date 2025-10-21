#include "common/common.hpp"
#include "common/module.hpp"
#include "common/affine.hpp"
#include "notify/notify.hpp"
#include "window/window.hpp"
#include "render/render.hpp"

#include <d3d9.h>
#include <d3d9helper.h>

namespace lim::render {
    static struct {
        IDirect3D9*          mContext   { nullptr       };
        IDirect3DDevice9*    mDevice    { nullptr       };
        IDirect3DSwapChain9* mSwapchain { nullptr       };
        Status               mStatus    { Status::EMPTY };
    } self;

    void _OnFence(const notify::Notify& msg) {
        if (self.mStatus != Status::ACTIVE) return;
        self.mDevice->Present(NULL, NULL, NULL, NULL);
        self.mDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0xFF111111, 0.0f, 0);
    };

    Status Probe(void) {
        return self.mStatus;
    };

    Status Create(void) {
        if (self.mStatus == Status::ACTIVE) {
            return self.mStatus;
        }

        if (window::Probe() != Status::ACTIVE) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        if (!(self.mContext = Direct3DCreate9(D3D_SDK_VERSION))) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        HWND    hwnd  = static_cast<HWND>(window::Handle());
        i32vec2 size  = window::GetSize();
        i32vec3 depth = window::GetDepth();

        D3DPRESENT_PARAMETERS frame {};
        frame.Windowed               = TRUE;
        frame.SwapEffect             = D3DSWAPEFFECT_DISCARD,
        frame.BackBufferWidth        = size.x;
        frame.BackBufferHeight       = size.y;
        frame.BackBufferCount        = 2;
        frame.BackBufferFormat       = D3DFMT_X8R8G8B8;
        frame.hDeviceWindow          = hwnd;
        frame.EnableAutoDepthStencil = TRUE;
        frame.AutoDepthStencilFormat = D3DFMT_D24S8;
        frame.PresentationInterval   = D3DPRESENT_INTERVAL_ONE;

        HRESULT result;

        result = self.mContext->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hwnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &frame,
            &self.mDevice
        );

        if (FAILED(result)) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        result = self.mDevice->GetSwapChain(
            0,
            &self.mSwapchain
        );

        if (FAILED(result)) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        notify::Attach(notify::eKindFENCE, _OnFence);

        self.mStatus = Status::ACTIVE;
        return self.mStatus;
    };

    void Update(void) {
        if (self.mStatus != Status::ACTIVE) {
            return;
        }
    };

    void Delete(void) {
        if (self.mStatus == Status::EMPTY) {
            return;
        }

        if (self.mSwapchain) self.mSwapchain->Release();
        if (self.mDevice)    self.mDevice->Release();
        if (self.mContext)   self.mContext->Release();

        self.mSwapchain = nullptr;
        self.mDevice = nullptr;
        self.mContext = nullptr;
        self.mStatus = Status::EMPTY;
    };
};