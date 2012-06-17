#include <stdio.h>
#include <string.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "x11.hpp"

namespace JSDXToolkit {

	void X11::setWindowDecorator(Display *disp, Window w, bool enabled)
	{
		unsigned char *data;
		MotifWmHints new_hints;
		MotifWmHints *hints;
		int format;
		unsigned long nitems;
		unsigned long bytes_after;
		Atom type;
		Atom hints_atom = XInternAtom(disp, "_MOTIF_WM_HINTS", False);
		XSetWindowAttributes attr;

		/* Get Current hints and set decorator flags */
		XGetWindowProperty(disp, w,
			hints_atom, 0, sizeof(MotifWmHints) / sizeof(long),
			False, AnyPropertyType, &type, &format, &nitems,
			&bytes_after, &data);

		if (type == None) {
			new_hints.flags = MWM_HINTS_DECORATIONS;
			new_hints.decorations = enabled ? MWM_DECOR_ALL : 0L;
			hints = &new_hints;
		} else {
			hints = (MotifWmHints *)data;
			hints->flags |= MWM_HINTS_DECORATIONS;

			if (enabled) {
				hints->decorations |= MWM_DECOR_ALL;
			} else {
				hints->decorations = 0L;
			}
		}

		XGrabServer(disp);
		XChangeProperty(disp, w,
			hints_atom, hints_atom, 32, PropModeReplace,
			(unsigned char *)hints, sizeof(MotifWmHints) / sizeof(long));
		XUngrabServer(disp);

		if (hints != &new_hints)
			XFree(hints);
	}

	void X11::setWindowType(Display *disp, Window w, X11WindowType wtype)
	{
		Atom wt_atom;
		Atom type_atom;

		switch(wtype) {
		case X11_WINDOW_TYPE_NORMAL:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_NORMAL", False);
			break;
		case X11_WINDOW_TYPE_DESKTOP:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
			break;
		case X11_WINDOW_TYPE_DOCK:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DOCK", False);
			break;
		case X11_WINDOW_TYPE_TOOLBAR:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
			break;
		case X11_WINDOW_TYPE_MENU:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_MENU", False);
			break;
		case X11_WINDOW_TYPE_UTILITY:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_UTILITY", False);
			break;
		case X11_WINDOW_TYPE_SPLASH:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_SPLASH", False);
			break;
		case X11_WINDOW_TYPE_DIALOG:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DIALOG", False);
			break;
		case X11_WINDOW_TYPE_POPUP_MENU:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_POPUP_MENU", False);
			break;
		default:
			type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_NORMAL", False);
		}

		wt_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE", False);

		XChangeProperty(disp, w,
			wt_atom,
			XA_ATOM, 32, PropModeReplace,
			(unsigned char *)&type_atom, 1);
	}

	void X11::windowConfigure(Display *disp, Window root, Window w, X11WindowType wtype)
	{
		XSetWindowAttributes attr;

		switch(wtype) {
		case X11_WINDOW_TYPE_NORMAL:
			break;
		case X11_WINDOW_TYPE_DESKTOP:
			break;
		case X11_WINDOW_TYPE_DOCK:
			attr.override_redirect = True;
			XChangeWindowAttributes(disp, w, CWOverrideRedirect, &attr);
			break;

		case X11_WINDOW_TYPE_TOOLBAR:
			break;

		case X11_WINDOW_TYPE_UTILITY:
			break;

		case X11_WINDOW_TYPE_SPLASH:
			break;

		case X11_WINDOW_TYPE_DIALOG:
			break;

		case X11_WINDOW_TYPE_MENU:
		case X11_WINDOW_TYPE_POPUP_MENU:

			skipTaskbarHint(disp, w, True);

			/* Transient window */
			attr.override_redirect = True;
			XChangeWindowAttributes(disp, w, CWOverrideRedirect, &attr);
			XSetTransientForHint(disp, w, root);
			break;

		default:
			break;
		}

	}

	void X11::setActive(Display *disp, Window root, Window w, bool active)
	{
		Atom atom = XInternAtom(disp, "_NET_ACTIVE_WINDOW", False);
		Atom wm_protocol = XInternAtom(disp, "WM_PROTOCOLS", False);
		Atom wm_take_focus = XInternAtom(disp, "WM_TAKE_FOCUS", False);

		if (active) {
			XSetInputFocus(disp, w, RevertToParent, CurrentTime);

			send_xclient_message(disp, w, wm_protocol, wm_take_focus, 0L);

			XChangeProperty(disp, root,
				atom, XA_WINDOW, 32, PropModeReplace,
				(unsigned char *)&w, 1);
		} else {
			XDeleteProperty(disp, root, atom);
		}
	}

	void X11::send_xclient_message(Display *disp, Window w, Atom a, long data0, long data1)
	{
		XEvent ev;

		memset(&ev, 0, sizeof(ev));
		ev.xclient.type = ClientMessage;
		ev.xclient.window = w;
		ev.xclient.message_type = a;
		ev.xclient.format = 32;
		ev.xclient.data.l[0] = data0;
		ev.xclient.data.l[1] = data1;

		XSendEvent(disp, w, False, 0L, (XEvent *) &ev);
	}

	void X11::reparentWindow(Display *disp, Window w, Window parent)
	{
		XReparentWindow(disp, w, parent, 0, 0);
	}

	void X11::skipTaskbarHint(Display *disp, Window w, bool skip)
	{
		Atom wm_state = XInternAtom(disp,  "WM_STATE", False);
		Atom atom = XInternAtom(disp, "_NET_WM_STATE_SKIP_TASKBAR", False);

		XChangeProperty(disp, w, wm_state,
			XA_ATOM, 32, PropModeAppend,
			(unsigned char *)&atom, 1);
	}

}
