#ifndef JSDX_TOOLKIT_BACKEND_X11_H_
#define JSDX_TOOLKIT_BACKEND_X11_H_

#include <X11/Xatom.h>
#include <X11/Xlib.h>

/* Window Hints */
#define MWM_HINTS_FUNCTIONS     (1L << 0)
#define MWM_HINTS_DECORATIONS   (1L << 1)
#define MWM_HINTS_INPUT_MODE    (1L << 2)
#define MWM_HINTS_STATUS        (1L << 3)

#define MWM_FUNC_ALL            (1L << 0)
#define MWM_FUNC_RESIZE         (1L << 1)
#define MWM_FUNC_MOVE           (1L << 2)
#define MWM_FUNC_MINIMIZE       (1L << 3)
#define MWM_FUNC_MAXIMIZE       (1L << 4)
#define MWM_FUNC_CLOSE          (1L << 5)

#define MWM_DECOR_ALL           (1L << 0)
#define MWM_DECOR_BORDER        (1L << 1)
#define MWM_DECOR_RESIZEH       (1L << 2)
#define MWM_DECOR_TITLE         (1L << 3)
#define MWM_DECOR_MENU          (1L << 4)
#define MWM_DECOR_MINIMIZE      (1L << 5)
#define MWM_DECOR_MAXIMIZE      (1L << 6)

namespace JSDXToolkit {

	namespace X11 {

		typedef struct {
			long flags;
			long functions;
			long decorations;
			long input_mode;
			long state;
		} MotifWmHints;

		typedef enum {
			X11_WINDOW_TYPE_NORMAL,
			X11_WINDOW_TYPE_DESKTOP,
			X11_WINDOW_TYPE_DOCK,
			X11_WINDOW_TYPE_TOOLBAR,
			X11_WINDOW_TYPE_MENU,
			X11_WINDOW_TYPE_UTILITY,
			X11_WINDOW_TYPE_SPLASH,
			X11_WINDOW_TYPE_DIALOG,
			X11_WINDOW_TYPE_POPUP_MENU
		} X11WindowType;

		void setWindowDecorator(Display *disp, Window w, bool enabled);
		void setWindowType(Display *disp, Window w, X11WindowType wtype);
		void windowConfigure(Display *disp, Window root, Window w, X11WindowType wtype);
		void setActive(Display *disp, Window root, Window w, bool active);
		void send_xclient_message(Display *disp, Window w, Atom a, long data0, long data1);
		void reparentWindow(Display *disp, Window w, Window parent);

		void skipTaskbarHint(Display *disp, Window w, bool skip);
	}

}

#endif
