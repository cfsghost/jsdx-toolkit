/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <ev.h>
#include <stdlib.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>

#include "jsdx_toolkit.hpp"
#include "actor.hpp"
#include "container.hpp"
#include "group.hpp"
#include "stage.hpp"
#include "rectangle.hpp"
#include "text.hpp"
#include "texture.hpp"
#include "state.hpp"
#include "media.hpp"
#include "application.hpp"
#include "window.hpp"

#if ENABLE_CLUTTER_GST
#include "gst/gst.hpp"
#include "gst/video-texture.hpp"
#endif

#if ENABLE_WIDGET
#include "widgets/widget.hpp"
//#include "widgets/FlickView.hpp"
#endif

namespace JSDXToolkit {
 
	using namespace node;
	using namespace v8;

	struct econtext {
		GPollFD *pfd;
		ev_io *iow;
		int nfd, afd;
		gint maxpri;

		ev_prepare pw;
		ev_check cw;
		ev_timer tw;

		GMainContext *gc;
	};

	static struct econtext default_context;

	static void timer_cb(EV_P_ ev_timer *w, int revents) {
	  /* nop */
	}

	static void io_cb(EV_P_ ev_io *w, int revents) {
	  /* nop */
	}

	static void prepare_cb(EV_P_ ev_prepare *w, int revents)
	{
		struct econtext *ctx = (struct econtext *)(((char *)w) - offsetof (struct econtext, pw));
		gint timeout;
		int i;

		g_main_context_dispatch(ctx->gc);

		g_main_context_prepare(ctx->gc, &ctx->maxpri);

		while(ctx->afd < (ctx->nfd = g_main_context_query(
			ctx->gc,
			ctx->maxpri,
			&timeout,
			ctx->pfd,
			ctx->afd))) {

			free (ctx->pfd);
			free (ctx->iow);

			ctx->afd = 1;
			while (ctx->afd < ctx->nfd)
				ctx->afd <<= 1;

			ctx->pfd = (GPollFD*) malloc(ctx->afd * sizeof (GPollFD));
			ctx->iow = (ev_io*) malloc(ctx->afd * sizeof (ev_io));
		}

		for (i = 0; i < ctx->nfd; ++i) {
			GPollFD *pfd = ctx->pfd + i;
			ev_io *iow = ctx->iow + i;

			pfd->revents = 0;

			ev_io_init(iow,
				io_cb,
				pfd->fd,
				(pfd->events & G_IO_IN ? EV_READ : 0)
				| (pfd->events & G_IO_OUT ? EV_WRITE : 0));

			iow->data = (void *)pfd;
			ev_set_priority(iow, EV_MINPRI);
			ev_io_start(EV_A_ iow);
		}

		if (timeout >= 0) {
			ev_timer_set(&ctx->tw, timeout * 1e-3, 0.);
			ev_timer_start(EV_A_ &ctx->tw);
		}
	}

	static void check_cb(EV_P_ ev_check *w, int revents)
	{
		struct econtext *ctx = (struct econtext *)(((char *)w) - offsetof (struct econtext, cw));
		int i;

		for (i = 0; i < ctx->nfd; ++i) {
			ev_io *iow = ctx->iow + i;

			if (ev_is_pending(iow)) {
				GPollFD *pfd = ctx->pfd + i;
				int revents = ev_clear_pending(EV_A_ iow);

				pfd->revents |= pfd->events & ((revents & EV_READ ? G_IO_IN : 0) | (revents & EV_WRITE ? G_IO_OUT : 0));
			}

			ev_io_stop(EV_A_ iow);
		}

		if (ev_is_active(&ctx->tw))
			ev_timer_stop(EV_A_ &ctx->tw);

		g_main_context_check(ctx->gc, ctx->maxpri, ctx->pfd, ctx->nfd);
	}

	static void ContextInit()
	{
		GMainContext *gc = g_main_context_default();
		struct econtext *ctx = &default_context;

		if (!g_thread_supported())
			g_thread_init(NULL);

		g_main_context_acquire(gc);
		ctx->gc = g_main_context_ref(gc);
		ctx->nfd = 0;
		ctx->afd = 0;
		ctx->iow = 0;
		ctx->pfd = 0;

		ev_prepare_init(&ctx->pw, prepare_cb);
		ev_set_priority(&ctx->pw, EV_MINPRI);
		ev_prepare_start(EV_DEFAULT_UC_ &ctx->pw);
		ev_unref(EV_DEFAULT_UC);

		ev_check_init(&ctx->cw, check_cb);
		ev_set_priority(&ctx->cw, EV_MAXPRI);
		ev_check_start(EV_DEFAULT_UC_ &ctx->cw);
		ev_unref(EV_DEFAULT_UC);

		ev_init(&ctx->tw, timer_cb);
		ev_set_priority(&ctx->tw, EV_MINPRI);
	} 

	static Handle<Value> ClutterInit(const Arguments& args)
	{
		HandleScope scope;

		return scope.Close(Integer::New(clutter_init(NULL, NULL)));
	}

	static Handle<Value> ClutterMain(const Arguments& args)
	{
		HandleScope scope;

		ev_ref(EV_DEFAULT_UC);

		return Undefined();
	}

	static Handle<Value> SetUseARGBVisual(const Arguments& args)
	{
		if (args[0]->IsBoolean()) {
			clutter_x11_set_use_argb_visual(args[0]->ToBoolean()->Value());
		}

		return Undefined();
	}

	static Handle<Value> ClutterMainQuit(const Arguments& args)
	{
		ev_unref(EV_DEFAULT_UC);

		return Undefined();
	}
	 
	extern "C" {
		static void init(Handle<Object> target)
		{
			HandleScope scope;

			/* Error conditions for initializing*/
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "INIT_SUCCESS", CLUTTER_INIT_SUCCESS);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "INIT_UNKNOWN", CLUTTER_INIT_ERROR_UNKNOWN);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "INIT_THREADS", CLUTTER_INIT_ERROR_THREADS);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "INIT_BACKEND", CLUTTER_INIT_ERROR_BACKEND);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "INIT_INTERNAL", CLUTTER_INIT_ERROR_INTERNAL);

			/* ClutterGravity */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "GRAVITY_NONE", CLUTTER_GRAVITY_NONE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "GRAVITY_NORTH", CLUTTER_GRAVITY_NORTH);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "GRAVITY_NORTH_EAST", CLUTTER_GRAVITY_NORTH_EAST);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "GRAVITY_EAST", CLUTTER_GRAVITY_EAST);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "GRAVITY_SOUTH_EAST", CLUTTER_GRAVITY_SOUTH_EAST);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "GRAVITY_SOUTH", CLUTTER_GRAVITY_SOUTH);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "GRAVITY_SOUTH_WEST", CLUTTER_GRAVITY_SOUTH_WEST);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "GRAVITY_CENTER", CLUTTER_GRAVITY_CENTER);

			/* ClutterRotateAxis */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "X_AXIS", CLUTTER_X_AXIS);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "Y_AXIS", CLUTTER_Y_AXIS);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "Z_AXIS", CLUTTER_Z_AXIS);

			/* ClutterRotateDirection */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "ROTATE_CW", CLUTTER_ROTATE_CW);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "ROTATE_CCW", CLUTTER_ROTATE_CCW);

			/* ClutterAlpha */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "CUSTOM_MODE", CLUTTER_CUSTOM_MODE);

			/* linear */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "LINEAR", CLUTTER_LINEAR);

			/* quadratic */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_QUAD", CLUTTER_EASE_IN_QUAD);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_QUAD", CLUTTER_EASE_OUT_QUAD);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_QUAD", CLUTTER_EASE_IN_OUT_QUAD);

			/* cubic */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_CUBIC", CLUTTER_EASE_IN_CUBIC);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_CUBIC", CLUTTER_EASE_OUT_CUBIC);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_CUBIC", CLUTTER_EASE_IN_OUT_CUBIC);

			/* quartic */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_QUART", CLUTTER_EASE_IN_QUART);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_QUART", CLUTTER_EASE_OUT_QUART);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_QUART", CLUTTER_EASE_IN_OUT_QUART);

			/* quintic */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_QUINT", CLUTTER_EASE_IN_QUINT);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_QUINT", CLUTTER_EASE_OUT_QUINT);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_QUINT", CLUTTER_EASE_IN_OUT_QUINT);

			/* sinusoidal */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_SINE", CLUTTER_EASE_IN_SINE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_SINE", CLUTTER_EASE_OUT_SINE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_SINE", CLUTTER_EASE_IN_OUT_SINE);

			/* exponential */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_EXPO", CLUTTER_EASE_IN_EXPO);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_EXPO", CLUTTER_EASE_OUT_EXPO);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_EXPO", CLUTTER_EASE_IN_OUT_EXPO);

			/* circular */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_CIRC", CLUTTER_EASE_IN_CIRC);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_CIRC", CLUTTER_EASE_OUT_CIRC);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_CIRC", CLUTTER_EASE_IN_OUT_CIRC);

			/* elastic */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_ELASTIC", CLUTTER_EASE_IN_ELASTIC);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_ELASTIC", CLUTTER_EASE_OUT_ELASTIC);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_ELASTIC", CLUTTER_EASE_IN_OUT_ELASTIC);

			/* overshooting cubic */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_BACK", CLUTTER_EASE_IN_BACK);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_BACK", CLUTTER_EASE_OUT_BACK);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_BACK", CLUTTER_EASE_IN_OUT_BACK);

			/* exponentially decaying parabolic */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_BOUNCE", CLUTTER_EASE_IN_BOUNCE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_OUT_BOUNCE", CLUTTER_EASE_OUT_BOUNCE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EASE_IN_OUT_BOUNCE", CLUTTER_EASE_IN_OUT_BOUNCE);

			/* Effect */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EFFECT_CUSTOM", JSDX_TOOLKIT_EFFECT_CUSTOM);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EFFECT_BLUR", JSDX_TOOLKIT_EFFECT_BLUR);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EFFECT_COLORIZE", JSDX_TOOLKIT_EFFECT_COLORIZE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EFFECT_DESATURATE", JSDX_TOOLKIT_EFFECT_DESATURATE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EFFECT_PAGE_TURN", JSDX_TOOLKIT_EFFECT_PAGE_TURN);

			/* own events */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_DESTROY", JSDX_TOOLKIT_EVENT_DESTROY);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_SHOW", JSDX_TOOLKIT_EVENT_SHOW);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_HIDE", JSDX_TOOLKIT_EVENT_HIDE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_PRESS", JSDX_TOOLKIT_EVENT_PRESS);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_RELEASE", JSDX_TOOLKIT_EVENT_RELEASE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_KEY_PRESS", JSDX_TOOLKIT_EVENT_KEY_PRESS);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_KEY_RELEASE", JSDX_TOOLKIT_EVENT_KEY_RELEASE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_CLICK", JSDX_TOOLKIT_EVENT_CLICK);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_LONG_PRESS", JSDX_TOOLKIT_EVENT_LONG_PRESS);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_ENTER", JSDX_TOOLKIT_EVENT_ENTER);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_LEAVE", JSDX_TOOLKIT_EVENT_LEAVE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_MOTION", JSDX_TOOLKIT_EVENT_MOTION);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_DRAG", JSDX_TOOLKIT_EVENT_DRAG);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "EVENT_SCROLL", JSDX_TOOLKIT_EVENT_SCROLL);

			/* Scroll direction */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "SCROLL_DIRECTION_UP", JSDX_TOOLKIT_SCROLL_UP);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "SCROLL_DIRECTION_DOWN", JSDX_TOOLKIT_SCROLL_DOWN);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "SCROLL_DIRECTION_LEFT", JSDX_TOOLKIT_SCROLL_LEFT);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "SCROLL_DIRECTION_RIGHT", JSDX_TOOLKIT_SCROLL_RIGHT);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "SCROLL_DIRECTION_SMOOTH", JSDX_TOOLKIT_SCROLL_SMOOTH);

			/* Action state */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "ACTION_STATE_LONG_PRESS_QUERY", CLUTTER_LONG_PRESS_QUERY);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "ACTION_STATE_LONG_PRESS_ACTIVATE", CLUTTER_LONG_PRESS_ACTIVATE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "ACTION_STATE_LONG_PRESS_CANCEL", CLUTTER_LONG_PRESS_CANCEL);

			/* Animation */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "ANIMATION_PLAY", JSDX_TOOLKIT_ANIMATION_PLAY);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "ANIMATION_PAUSE", JSDX_TOOLKIT_ANIMATION_PAUSE);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "ANIMATION_STOP", JSDX_TOOLKIT_ANIMATION_STOP);

			/* Window Type */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "WINDOW_TYPE_NORMAL", JSDX_WINDOW_TYPE_NORMAL);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "WINDOW_TYPE_DESKTOP", JSDX_WINDOW_TYPE_DESKTOP);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "WINDOW_TYPE_DOCK", JSDX_WINDOW_TYPE_DOCK);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "WINDOW_TYPE_TOOLBAR", JSDX_WINDOW_TYPE_TOOLBAR);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "WINDOW_TYPE_MENU", JSDX_WINDOW_TYPE_MENU);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "WINDOW_TYPE_UTILITY", JSDX_WINDOW_TYPE_UTILITY);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "WINDOW_TYPE_SPLASH", JSDX_WINDOW_TYPE_SPLASH);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "WINDOW_TYPE_DIALOG", JSDX_WINDOW_TYPE_DIALOG);
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "WINDOW_TYPE_POPUP_MENU", JSDX_WINDOW_TYPE_POPUP_MENU);

			/* Texture Type */
			JSDX_TOOLKIT_DEFINE_CONSTANT(target, "TEXTURE_X11", JSDX_TOOLKIT_TEXTURE_X11);

			NODE_SET_METHOD(target, "init", ClutterInit);
			NODE_SET_METHOD(target, "main", ClutterMain);
			NODE_SET_METHOD(target, "quit", ClutterMainQuit);
			NODE_SET_METHOD(target, "useARGB", SetUseARGBVisual);

			/* Application */
			Application::Initialize(target);
			JSDXWindow::Initialize(target);

			Actor::Initialize(target);
			Container::Initialize(target);
			Group::Initialize(target);
			Stage::Initialize(target);
			Rectangle::Initialize(target);
			Text::Initialize(target);
			Texture::Initialize(target);
			State::Initialize(target);

#if ENABLE_CLUTTER_GST
			Gst::Initialize(target);
			GstVideoTexture::Initialize(target);
#endif

#if ENABLE_WIDGET
			Widget::Initialize(target);
//			FlickView::Initialize(target);
#endif

			ContextInit();
		}

		NODE_MODULE(jsdx_toolkit, init);
	}

}
