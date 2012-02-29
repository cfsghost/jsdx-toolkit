var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'Drag Action';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.opacity = 150;
texture1.reactive(true);
texture1.on(clutter.EVENT_DRAG, function(ev, data) {
	console.log(ev);
	console.log(data);
});
stage.add(texture1);

var texture2 = new clutter.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(200, 200);
texture2.depth = -200;
texture2.reactive(true);
texture2.on(clutter.EVENT_DRAG, { x_threshold: 50, y_threshold: 50 }, function(ev, data) {
	console.log(ev);
	console.log(data);
});
stage.add(texture2);

clutter.main();
