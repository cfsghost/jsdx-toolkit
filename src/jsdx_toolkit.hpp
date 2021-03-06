#ifndef JSDX_TOOLKIT_H_
#define JSDX_TOOLKIT_H_

#include <v8.h>
#include <clutter/clutter.h>

namespace JSDXToolkit {

#define JSDX_TOOLKIT_DEFINE_CONSTANT(target, name, constant)					\
	(target)->Set(v8::String::NewSymbol(name),							\
	v8::Integer::New(constant),											\
	static_cast<v8::PropertyAttribute>(v8::ReadOnly|v8::DontDelete))

	typedef enum {
		JSDX_TOOLKIT_EVENT_DESTROY,
		JSDX_TOOLKIT_EVENT_SHOW,
		JSDX_TOOLKIT_EVENT_HIDE,
		JSDX_TOOLKIT_EVENT_PRESS,
		JSDX_TOOLKIT_EVENT_RELEASE,
		JSDX_TOOLKIT_EVENT_KEY_PRESS,
		JSDX_TOOLKIT_EVENT_KEY_RELEASE,
		JSDX_TOOLKIT_EVENT_CLICK,
		JSDX_TOOLKIT_EVENT_LONG_PRESS,
		JSDX_TOOLKIT_EVENT_ENTER,
		JSDX_TOOLKIT_EVENT_LEAVE,
		JSDX_TOOLKIT_EVENT_MOTION,
		JSDX_TOOLKIT_EVENT_DRAG,
		JSDX_TOOLKIT_EVENT_SCROLL
	} JSDXToolkitEvent;

	typedef enum {
		JSDX_TOOLKIT_SCROLL_UP,
		JSDX_TOOLKIT_SCROLL_DOWN,
		JSDX_TOOLKIT_SCROLL_LEFT,
		JSDX_TOOLKIT_SCROLL_RIGHT,
		JSDX_TOOLKIT_SCROLL_SMOOTH
	} JSDXToolkitScrollDirection;

	typedef enum {
		JSDX_TOOLKIT_EFFECT_CUSTOM,
		JSDX_TOOLKIT_EFFECT_BLUR,
		JSDX_TOOLKIT_EFFECT_COLORIZE,
		JSDX_TOOLKIT_EFFECT_DESATURATE,
		JSDX_TOOLKIT_EFFECT_PAGE_TURN
	} JSDXToolkitEffect;

	typedef enum {
		JSDX_TOOLKIT_TEXTURE_X11
	} JSDXTookitTextureType;

	struct NodeCallback {
		v8::Persistent<v8::Object> Holder;
		v8::Persistent<v8::Function> cb;

		~NodeCallback() {
			Holder.Dispose();
			cb.Dispose();
		}
	};
}

#endif
