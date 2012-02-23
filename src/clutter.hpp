#ifndef NODE_CLUTTER_H_
#define NODE_CLUTTER_H_

#include <v8.h>
#include <clutter/clutter.h>

namespace clutter {

#define CLUTTER_DEFINE_CONSTANT(target, name, constant)					\
	(target)->Set(v8::String::NewSymbol(name),							\
	v8::Integer::New(constant),											\
	static_cast<v8::PropertyAttribute>(v8::ReadOnly|v8::DontDelete))

typedef enum {
	NODE_CLUTTER_EVENT_DESTROY,
	NODE_CLUTTER_EVENT_SHOW,
	NODE_CLUTTER_EVENT_HIDE,
	NODE_CLUTTER_EVENT_CLICK,
	NODE_CLUTTER_EVENT_LONG_PRESS,
	NODE_CLUTTER_EVENT_ENTER,
	NODE_CLUTTER_EVENT_LEAVE,
	NODE_CLUTTER_EVENT_MOTION
} NodeClutterEvent;

typedef enum {
	NODE_CLUTTER_EFFECT_CUSTOM,
	NODE_CLUTTER_EFFECT_BLUR,
	NODE_CLUTTER_EFFECT_COLORIZE,
	NODE_CLUTTER_EFFECT_DESATURATE,
	NODE_CLUTTER_EFFECT_PAGE_TURN
} NodeClutterEffect;

}

#endif
