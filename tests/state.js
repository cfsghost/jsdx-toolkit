var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'State Manchine';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* Create two actors */
var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.reactive(true);
texture1.scale(0.5, 0.5, clutter.GRAVITY_CENTER);
stage.add(texture1);

var texture2 = new clutter.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(200, 200);
texture2.scale(0.5, 0.5, clutter.GRAVITY_CENTER);
stage.add(texture2);

/* Create state manchine */
var state = new clutter.State(400);
state.set('focus', [
	[ texture1, 'scale-x', clutter.EASE_OUT_CUBIC, 1.0 ],
	[ texture1, 'scale-y', clutter.EASE_OUT_CUBIC, 1.0 ],
	[ texture2, 'scale-x', clutter.EASE_OUT_CUBIC, 1.5 ],
	[ texture2, 'scale-y', clutter.EASE_OUT_CUBIC, 1.5 ]
]);

state.set('blur', [
	[ texture1, 'scale-x', clutter.EASE_OUT_CUBIC, 0.5 ],
	[ texture1, 'scale-y', clutter.EASE_OUT_CUBIC, 0.5 ],
	[ texture2, 'scale-x', clutter.EASE_OUT_CUBIC, 0.5 ],
	[ texture2, 'scale-y', clutter.EASE_OUT_CUBIC, 0.5 ]
]);

texture1.on(clutter.EVENT_ENTER, function(ev) {
	state.setState('focus');

	return true;
});

texture1.on(clutter.EVENT_LEAVE, function(ev) {
	state.setState('blur');

	return true;
});


clutter.main();
