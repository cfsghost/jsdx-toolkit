#ifndef JDDX_TOOLKIT_H_
#define JDDX_TOOLKIT_H_

#include <v8.h>
#include <clutter/clutter.h>

namespace JSDXToolkit {

#define JSDX_TOOLKIT_DEFINE_CONSTANT(target, name, constant)					\
	(target)->Set(v8::String::NewSymbol(name),							\
	v8::Integer::New(constant),											\
	static_cast<v8::PropertyAttribute>(v8::ReadOnly|v8::DontDelete))

	typedef enum {
		JDDX_TOOLKIT_EVENT_DESTROY,
		JDDX_TOOLKIT_EVENT_SHOW,
		JDDX_TOOLKIT_EVENT_HIDE,
		JDDX_TOOLKIT_EVENT_PRESS,
		JDDX_TOOLKIT_EVENT_RELEASE,
		JDDX_TOOLKIT_EVENT_CLICK,
		JDDX_TOOLKIT_EVENT_LONG_PRESS,
		JDDX_TOOLKIT_EVENT_ENTER,
		JDDX_TOOLKIT_EVENT_LEAVE,
		JDDX_TOOLKIT_EVENT_MOTION,
		JDDX_TOOLKIT_EVENT_DRAG
	} NodeClutterEvent;

	typedef enum {
		JDDX_TOOLKIT_EFFECT_CUSTOM,
		JDDX_TOOLKIT_EFFECT_BLUR,
		JDDX_TOOLKIT_EFFECT_COLORIZE,
		JDDX_TOOLKIT_EFFECT_DESATURATE,
		JDDX_TOOLKIT_EFFECT_PAGE_TURN
	} NodeClutterEffect;

}

#endif
