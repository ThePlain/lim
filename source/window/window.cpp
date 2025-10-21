#include "common/common.hpp"
#include "common/module.hpp"
#include "common/string.hpp"
#include "common/affine.hpp"
#include "notify/notify.hpp"
#include "window/window.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"

namespace lim::window {
    using Window    = GLFWwindow*;
    using Monitor   = GLFWmonitor*;
    using VideoMode = const GLFWvidmode*;

    static const GLFWvidmode DUMMYMODE { 1920, 1080, 8, 8, 8, 60 };

    static struct {
        i32vec2   mResolution { 1920, 1080    };
        i32vec2   mOffset     { 0, 0          };
        f32vec2   mScaling    { 1.0f, 1.0f    };
        String    mTitle      { "[LIM]"       };
        Monitor   mMonitor    { nullptr       };
        VideoMode mVideoMode  { &DUMMYMODE    };
        Window    mWindow     { nullptr       };
        Status    mStatus     { Status::EMPTY };
    } self;

    void _OnQuit(Window wnd) {
        notify::Notify msg;

        msg.kind = notify::eKindQUIT;
        msg.quit.reason = 0;

        notify::Dispatch(msg);
    };

    Status Probe(void) {
        return self.mStatus;
    };

    Status Create(void) {
        if (self.mStatus == Status::ACTIVE) {
            return self.mStatus;
        }

        if (!glfwInit()) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        if (!(self.mMonitor = glfwGetPrimaryMonitor())) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        if (!(self.mVideoMode = glfwGetVideoMode(self.mMonitor))) {
            self.mStatus    = Status::FAILED;
            self.mVideoMode = &DUMMYMODE;
            return self.mStatus;
        }

        glfwWindowHint(GLFW_CLIENT_API,   GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE,    GLFW_FALSE);
        glfwWindowHint(GLFW_REFRESH_RATE, self.mVideoMode->refreshRate);

        if (!(self.mWindow = glfwCreateWindow(self.mResolution.x, self.mResolution.y, self.mTitle.c_str(), NULL, NULL))) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        self.mOffset.x = (self.mVideoMode->width  - self.mResolution.x) / 2;
        self.mOffset.y = (self.mVideoMode->height - self.mResolution.y) / 2;

        glfwSetWindowCloseCallback(self.mWindow, _OnQuit);
        glfwGetWindowContentScale(self.mWindow, &self.mScaling.x, &self.mScaling.y);
        glfwSetWindowPos(self.mWindow, self.mOffset.x, self.mOffset.y);
        glfwShowWindow(self.mWindow);

        self.mStatus = Status::ACTIVE;

        return self.mStatus;
    };

    void Update(void) {
        if (self.mStatus != Status::ACTIVE) {
            return;
        }

        glfwPollEvents();
    };

    void Delete(void) {
        if (self.mStatus == Status::EMPTY) {
            return;
        }

        if (self.mWindow)
            glfwDestroyWindow(self.mWindow);
        glfwTerminate();

        self.mWindow    = nullptr;
        self.mVideoMode = &DUMMYMODE;
        self.mMonitor   = nullptr;
        self.mStatus    = Status::EMPTY;
    };

    void* Handle(void) {
        return self.mStatus == Status::ACTIVE ? glfwGetWin32Window(self.mWindow) : NULL;
    };

    i32vec2 GetSize(void) {
        return self.mResolution;
    };

    i32vec3 GetDepth(void) {
        return i32vec3 {
            self.mVideoMode->redBits,
            self.mVideoMode->greenBits,
            self.mVideoMode->blueBits,
        };
    };
};