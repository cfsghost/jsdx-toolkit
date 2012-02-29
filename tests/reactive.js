var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'Reactive';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.reactive(true);
stage.add(texture1);

clutter.main();
