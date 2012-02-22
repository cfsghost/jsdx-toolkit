#ifndef NODE_CLUTTER_H_
#define NODE_CLUTTER_H_

#include <v8.h>
#include <clutter/clutter.h>

namespace clutter {

#define CLUTTER_DEFINE_CONSTANT(target, name, constant)					\
	(target)->Set(v8::String::NewSymbol(name),							\
	v8::Integer::New(constant),											\
	static_cast<v8::PropertyAttribute>(v8::ReadOnly|v8::DontDelete))

}

#endif
