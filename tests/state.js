var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'State Manchine';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

/* Create two actors */
var texture1 = new toolkit.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.reactive = true;
texture1.scale(0.5, 0.5, toolkit.GRAVITY_CENTER);
stage.add(texture1);

var texture2 = new toolkit.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(200, 200);
texture2.scale(0.5, 0.5, toolkit.GRAVITY_CENTER);
stage.add(texture2);

/* Create state manchine */
var state = new toolkit.State(400);
state.set('focus', [
	[ texture1, 'scale-x', toolkit.EASE_OUT_CUBIC, 1.0 ],
	[ texture1, 'scale-y', toolkit.EASE_OUT_CUBIC, 1.0 ],
	[ texture2, 'scale-x', toolkit.EASE_OUT_CUBIC, 1.5 ],
	[ texture2, 'scale-y', toolkit.EASE_OUT_CUBIC, 1.5 ]
]);

state.set('blur', [
	[ texture1, 'scale-x', toolkit.EASE_OUT_CUBIC, 0.5 ],
	[ texture1, 'scale-y', toolkit.EASE_OUT_CUBIC, 0.5 ],
	[ texture2, 'scale-x', toolkit.EASE_OUT_CUBIC, 0.5 ],
	[ texture2, 'scale-y', toolkit.EASE_OUT_CUBIC, 0.5 ]
]);

texture1.on('enter', function(ev) {
	state.setState('focus');

	return true;
});

texture1.on('leave', function(ev) {
	state.setState('blur');

	return true;
});


toolkit.main();
