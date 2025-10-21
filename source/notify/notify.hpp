#ifndef LIM_NOTIFY
#define LIM_NOTIFY

#include "common/common.hpp"
#include "common/module.hpp"
#include "common/affine.hpp"
#include "common/string.hpp"

namespace lim::notify {
    enum Kind : u32 {
        eKindUNKNOWN,
        eKindVISIBLE,
        eKindACTION,
        eKindRESIZE,
        eKindRESET,
        eKindFOCUS,
        eKindFENCE,
        eKindQUIT,
        eKindTEXT,
        eKindKEYUP,
        eKindKEYDOWN,
        eKindMOUSEUP,
        eKindMOUSEDOWN,
        eKindMOUSEMOVE,
        eKindMOUSEWHEEL,
        eKindCOUNT,
    };

    enum Key : u32 {
        eKeySpace,
        eKeyApostrophe,
        eKeyComma,
        eKeyMinus,
        eKeyPeriod,
        eKeySlash,
        eKey0,
        eKey1,
        eKey2,
        eKey3,
        eKey4,
        eKey5,
        eKey6,
        eKey7,
        eKey8,
        eKey9,
        eKeySemicolon,
        eKeyEqual,
        eKeyA,
        eKeyB,
        eKeyC,
        eKeyD,
        eKeyE,
        eKeyF,
        eKeyG,
        eKeyH,
        eKeyI,
        eKeyJ,
        eKeyK,
        eKeyL,
        eKeyM,
        eKeyN,
        eKeyO,
        eKeyP,
        eKeyQ,
        eKeyR,
        eKeyS,
        eKeyT,
        eKeyU,
        eKeyV,
        eKeyW,
        eKeyX,
        eKeyY,
        eKeyZ,
        eKeyLeftBracket,
        eKeyRightBracket,
        eKeyBackslash,
        eKeyGraveAccent,
        eKeyWorld1,
        eKeyWorld2,
        eKeyEscape,
        eKeyEnter,
        eKeyTab,
        eKeyBackspace,
        eKeyInsert,
        eKeyDelete,
        eKeyRight,
        eKeyLeft,
        eKeyDown,
        eKeyUp,
        eKeyPageUp,
        eKeyPageDown,
        eKeyHome,
        eKeyEnd,
        eKeyCapsLock,
        eKeyScrollLock,
        eKeyNumLock,
        eKeyPrintScreen,
        eKeyPause,
        eKeyF1,
        eKeyF2,
        eKeyF3,
        eKeyF4,
        eKeyF5,
        eKeyF6,
        eKeyF7,
        eKeyF8,
        eKeyF9,
        eKeyF10,
        eKeyF11,
        eKeyF12,
        eKeyF13,
        eKeyF14,
        eKeyF15,
        eKeyF16,
        eKeyF17,
        eKeyF18,
        eKeyF19,
        eKeyF20,
        eKeyF21,
        eKeyF22,
        eKeyF23,
        eKeyF24,
        eKeyF25,
        eKeyNum0,
        eKeyNum1,
        eKeyNum2,
        eKeyNum3,
        eKeyNum4,
        eKeyNum5,
        eKeyNum6,
        eKeyNum7,
        eKeyNum8,
        eKeyNum9,
        eKeyNumDecimal,
        eKeyNumDivide,
        eKeyNumMultiply,
        eKeyNumSubstract,
        eKeyNumAdd,
        eKeyNumEnter,
        eKeyNumEqual,
        eKeyLeftShift,
        eKeyLeftControl,
        eKeyLeftAlt,
        eKeyLeftSuper,
        eKeyRightShift,
        eKeyRightControl,
        eKeyRightAlt,
        eKeyRightSuper,
        eKeyMenu,
        eKeyMouse0,
        eKeyMouse1,
        eKeyMouse2,
        eKeyMouse3,
        eKeyMouse4,
        eKeyMouse5,
        eKeyMouse6,
        eKeyMouse7,
        eKeyJoyKey0,
        eKeyJoyKey1,
        eKeyJoyKey2,
        eKeyJoyKey3,
        eKeyJoyKey4,
        eKeyJoyKey5,
        eKeyJoyKey6,
        eKeyJoyKey7,
        eKeyJoyKey8,
        eKeyJoyKey9,
        eKeyJoyKey10,
        eKeyJoyKey11,
        eKeyJoyKey12,
        eKeyJoyKey13,
        eKeyJoyKey14,
        eKeyJoyKey15,
        eKeyJoyAxis0,
        eKeyJoyAxis1,
        eKeyJoyAxis2,
        eKeyJoyAxis3,
        eKeyJoyAxis4,
        eKeyJoyAxis5,
        eKeyCOUNT,
        eKeyINVALID,
        eKeyMouseLeft   = eKeyMouse0,
        eKeyMouseRight  = eKeyMouse1,
        eKeyMouseMiddle = eKeyMouse2,
    };

    struct Notify;
    struct Visible;
    struct Action;
    struct Resize;
    struct Reset;
    struct Focus;
    struct Fence;
    struct Quit;
    struct Text;
    struct KeyUp;
    struct KeyDown;
    struct MouseUp;
    struct MouseDown;
    struct MouseMove;
    struct MouseWheel;

    using Callback = void(*)(const Notify&);

    Status Probe(void);
    Status Create(void);
    void   Update(void);
    void   Delete(void);
    void   Attach(Kind kind, Callback clbk);
    void   Detach(Kind kind, Callback clbk);
    bool   Immediate(const Notify& notify);
    void   Dispatch(const Notify& notify);
    void   Suppress(void);

    struct Visible {
        bool status;
    };

    struct Action {
        char    action[32];
        bool    repeat;
        bool    pressed;
        float   value;
        f32vec2 position;
    };

    struct Resize {
        i32vec2 current;
        i32vec2 delta;
    };

    struct Reset {
    };

    struct Focus {
        bool state;
    };

    struct Fence {
    };

    struct Quit {
        i32 reason;
    };

    struct Text {
        char   value[16];
        u32    symbol;
    };

    struct KeyUp {
        Key code;
    };

    struct KeyDown {
        Key code;
    };

    struct MouseUp {
        Key code;
    };

    struct MouseDown {
        Key code;
    };

    struct MouseMove {
        f32vec2 position;
    };

    struct MouseWheel {
        f32vec2 position;
    };

    struct Notify {
        Kind kind;
        u64  frame;
        union {
            Visible    visible;
            Action     action;
            Resize     resize;
            Reset      reset;
            Focus      focus;
            Fence      fence;
            Quit       quit;
            Text       text;
            KeyUp      keyup;
            KeyDown    keydown;
            MouseUp    mouseup;
            MouseDown  mousedown;
            MouseMove  mousemove;
            MouseWheel mousewheel;
        };
    };

    static_assert(sizeof(Notify) <= 64, "U tries to waste memory?");
    /**
     * NOTE: Yep we use limit to 64 bytes per notify
     * Yep i know that it can be bigger
     * BUT we have preallocated ring buffer for it up to 4096 item
     */
};

#endif