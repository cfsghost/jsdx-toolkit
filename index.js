var clutter = module.exports = require('./build/Release/clutter');

var handlers = [];

handlers[clutter.EVENT_CLICK] = null;
handlers[clutter.EVENT_LONG_PRESS] = null;

var eventDispatcher = function(ev) {
	switch(ev) {
	case clutter.EVENT_CLICK:
		handlers[ev]();
		break;
	case clutter.EVENT_LONG_PRESS:
		/* TODO: Implement handler */
		break;
	}
};

clutter.Actor.on = function(ev, callback) {
	handlers[ev] = callback;

	/* Connect to event dispatcher */
	clutter._on(ev, eventDispatcher);
};

