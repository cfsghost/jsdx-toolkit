var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'Flick';
stage.resize(1000, 800);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.opacity = 150;
texture1.reactive = true;
stage.add(texture1);

var flickable = {
	width: 1000,
	height: 800
};

var deceleration = 0.3;
var stopFactor = 100;
var dx = 0;
var dy = 0;
var targetDx = 0;
var targetDy = 0;
var timeX;
var timeY;
var lastTimestampX;
var lastTimestampY;

/* Do not speed up with long press */
texture1.on(clutter.EVENT_LONG_PRESS, { duration: stopFactor }, function(ev, data) {
	switch(data.state) {
	case clutter.ACTION_STATE_LONG_PRESS_QUERY:
		return true;

	case clutter.ACTION_STATE_LONG_PRESS_ACTIVATE:
		targetDx = 0;
		targetDy = 0;
		return true;

	case clutter.ACTION_STATE_LONG_PRESS_CANCEL:
		return false;

	}

});

texture1.on(clutter.EVENT_PRESS, function(ev, data) {
	texture1.setAnimate(clutter.ANIMATION_PAUSE);
	timeX = new Date().getTime();
	timeY = timeX;
	dx = 0;
	dy = 0;
});

texture1.on(clutter.EVENT_DRAG, function(ev, data) {
	var durationX;
	var durationY;

	switch(ev) {
	case 'begin':
		break;

	case 'end':
		if (new Date().getTime() - lastTimestampX >= stopFactor || new Date().getTime() - lastTimestampY >= stopFactor) {
			dx = 0;
			dy = 0;
			targetDx = 0;
			targetDy = 0;
			break;
		}

		durationX = new Date().getTime() - timeX;
		durationY = new Date().getTime() - timeY;

		/* Assume a minimal duration */
		if (durationX == 0) durationX = 1000;
		if (durationY == 0) durationY = 1000;

		/* Accumulation */
		targetDx += Math.round(1000 / durationX) * dx;
		targetDy += Math.round(1000 / durationY) * dy;

		texture1.animate(clutter.EASE_OUT_CUBIC, 1000, {
			x: texture1.x + targetDx * deceleration,
			y: texture1.y + targetDy * deceleration
		});

		break;

	case 'motion':
		lastTimestampX = new Date().getTime();
		lastTimestampY = new Date().getTime();
		console.log(lastTimestampX);

		/* Reset delta and timer if they have different sign */
		if (((dx >= 0 && data.delta_x >= 0) || (dx <= 0 && data.delta_x <= 0)) &&
			((targetDx >= 0 && data.delta_x >= 0) || (targetDx <= 0 && data.delta_x <= 0))) {
			dx += data.delta_x;
		} else {
			dx = data.delta_x;
			timeX = lastTimestampX;
			targetDx = 0;
			targetDy = 0;
		}

		if (((dy >= 0 && data.delta_y >= 0) || (dy <= 0 && data.delta_y <= 0)) &&
			((targetDy >= 0 && data.delta_y >= 0) || (targetDy <= 0 && data.delta_y <= 0))) {
			lastDy = data.delta_y;
			dy += data.delta_y;
		} else {
			dy = data.delta_y;
			timeY = lastTimestampY;
			targetDx = 0;
			targetDy = 0;
		}

		break;
	}
});

var texture2 = new clutter.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(200, 200);
texture2.depth = -200;
texture2.reactive = true;
texture2.on(clutter.EVENT_DRAG, { x_threshold: 50, y_threshold: 50 }, function(ev, data) {
	console.log(ev);
	console.log(data);
});
stage.add(texture2);

//console.log(texture2.getX);

clutter.main();
