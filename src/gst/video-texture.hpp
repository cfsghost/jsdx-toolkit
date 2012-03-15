#ifndef NODE_CLUTTER_VIDEO_TEXTURE_H_
#define NODE_CLUTTER_VIDEO_TEXTURE_H_

#include <v8.h>
#include <node.h>

namespace JSDXToolkit {

class GstVideoTexture : public Gst {
public:
	static void Initialize(v8::Handle<v8::Object> target);

protected:
	GstVideoTexture();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
};

}

#endif
