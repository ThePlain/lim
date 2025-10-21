#include "common/common.hpp"
#include "common/module.hpp"
#include "common/affine.hpp"
#include "common/string.hpp"
#include "notify/notify.hpp"

#include <list>
#include <array>

namespace lim::notify {
    using Methods = std::list<Callback>;
    using Handles = std::array<Methods, eKindCOUNT>;
    using Buffer  = std::array<Notify,  4096>;

    static struct {
        Handles   mHandles  {               };
        Buffer    mBuffer   {               };
        size_t    mBase     { 0             };
        size_t    mUsed     { 0             };
        bool      mSuppress { false         };
        Status    mStatus   { Status::EMPTY };
    } self;

    Status Probe(void) {
        return self.mStatus;
    };

    Status Create(void) {
        if (self.mStatus == Status::ACTIVE) {
            return self.mStatus;
        }

        self.mBase   = 0;
        self.mUsed   = 0;
        self.mStatus = Status::ACTIVE;

        return self.mStatus;
    };

    void Update(void) {
        while (self.mUsed) {
            const Notify& notify = self.mBuffer[self.mBase];
            Immediate(notify);
            self.mBase += 1;
            self.mUsed -= 1;
            self.mBase %= self.mBuffer.size();

            if (notify.kind == eKindFENCE)
                break;
        }
    };

    void Delete(void) {
        if (self.mStatus == Status::EMPTY) {
            return;
        }

        for (Methods& methods : self.mHandles)
            methods.clear();
        self.mBase = 0;
        self.mUsed = 0;
        self.mStatus = Status::EMPTY;
    };

    void Attach(Kind kind, Callback clbk) {
        self.mHandles[kind].push_back(clbk);
    };

    void Detach(Kind kind, Callback clbk) {
        self.mHandles[kind].remove(clbk);
    };

    bool Immediate(const Notify& notify) {
        self.mSuppress = false;
        for (const Callback& clbk : self.mHandles[notify.kind]) {
            clbk(notify);
            if (self.mSuppress) break;
        }
        return self.mSuppress;
    };

    void Dispatch(const Notify& notify) {
        if (self.mUsed == self.mBuffer.size())
            return;
        size_t offset = (self.mBase + self.mUsed) % self.mBuffer.size();
        self.mBuffer[offset] = notify;
        self.mUsed++;
    };

    void Suppress(void) {
        self.mSuppress = true;
    };
};