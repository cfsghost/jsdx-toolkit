var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Drag Action';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(toolkit.EVENT_DESTROY, function() {
	toolkit.quit();
});
stage.show();

var texture1 = new toolkit.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.opacity = 150;
texture1.reactive = true;
texture1.on(toolkit.EVENT_DRAG, function(ev, data) {
	console.log(ev);
	console.log(data);
});
stage.add(texture1);

var texture2 = new toolkit.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(200, 200);
texture2.depth = -200;
texture2.reactive = true;
texture2.on(toolkit.EVENT_DRAG, { x_threshold: 50, y_threshold: 50 }, function(ev, data) {
	console.log(ev);
	console.log(data);
});
stage.add(texture2);

toolkit.main();
