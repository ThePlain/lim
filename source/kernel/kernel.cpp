#include "common/common.hpp"
#include "common/module.hpp"
#include "kernel/kernel.hpp"
#include "notify/notify.hpp"
#include "window/window.hpp"
#include "render/render.hpp"

namespace lim::kernel {
    static struct {
        size_t mFrameID { 0             };
        Status mStatus  { Status::EMPTY };
        bool   mActive  { false         };
    } self;

    void OnQuit(const notify::Notify& msg) {
        self.mActive = false;
    };

    Status Probe(void) {
        return self.mStatus;
    };

    Status Create(void) {
        if (self.mStatus == Status::ACTIVE) {
            return self.mStatus;
        }

        if (notify::Create() != Status::ACTIVE) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        if (window::Create() != Status::ACTIVE) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        if (render::Create() != Status::ACTIVE) {
            self.mStatus = Status::FAILED;
            return self.mStatus;
        }

        notify::Attach(notify::eKindQUIT, OnQuit);

        self.mFrameID = 0;
        self.mStatus  = Status::ACTIVE;
        self.mActive  = true;

        return self.mStatus;
    };

    void Update(void) {
        if (self.mStatus != Status::ACTIVE) {
            return;
        }

        notify::Update();
        window::Update();
        render::Update();
        notify::Dispatch({notify::eKindFENCE});

        self.mFrameID++;
    };

    void Delete(void) {
        if (self.mStatus == Status::EMPTY) {
            return;
        }

        render::Delete();
        window::Delete();
        notify::Delete();

        self.mFrameID = 0;
        self.mActive  = false;
        self.mStatus  = Status::EMPTY;
    };

    bool Active(void) {
        return self.mActive;
    };
};