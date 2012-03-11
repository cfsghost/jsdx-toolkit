#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include "../clutter.hpp"
#include "../actor.hpp"
#include "widget.hpp"
#include "adjustment.hpp"
#include "scrollable.hpp"
#include "button.hpp"
#include "frame.hpp"
#include "label.hpp"
#include "slider.hpp"
#include "toggle.hpp"
#include "spinner.hpp"
#include "image.hpp"
#include "bin.hpp"
#include "grid.hpp"
#include "table.hpp"
#include "viewport.hpp"
#include "scrollview.hpp"
#include "kinetic_scrollview.hpp"

namespace clutter {
 
	using namespace node;
	using namespace v8;

	Widget::Widget() : Actor() {}

	void Widget::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("Widget");

		/* Create a new class */
		Handle<ObjectTemplate> ObjectTpl = ObjectTemplate::New();
		ObjectTpl->SetInternalFieldCount(1);
		Local<Object> ClassObject = ObjectTpl->NewInstance();
		target->Set(name, ClassObject);

		/* Initializing Widget Class  */
		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		/* Widgets */
		Bin::Initialize(ClassObject);
		Grid::Initialize(ClassObject);
		Table::Initialize(ClassObject);
		Adjustment::Initialize(ClassObject);
		Scrollable::Initialize(ClassObject);
		Button::Initialize(ClassObject);
		Frame::Initialize(ClassObject);
		Label::Initialize(ClassObject);
		Slider::Initialize(ClassObject);
		Toggle::Initialize(ClassObject);
		Spinner::Initialize(ClassObject);
		Image::Initialize(ClassObject);
		Viewport::Initialize(ClassObject);
		ScrollView::Initialize(ClassObject);
		KineticScrollView::Initialize(ClassObject);

		ClassObject->Set(name, tpl->GetFunction());
	}

	void Widget::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Actor::PrototypeMethodsInit(constructor_template);
	}

	/* ECMAScript constructor */
	Handle<Value> Widget::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Widget* obj = new Widget();
		obj->Wrap(args.This());

		return scope.Close(args.This());
	}
}
