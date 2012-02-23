var clutter = require('../build/Release/clutter');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('Fullscreen');
stage.setColor(0, 0, 0, 255);
stage.fullscreen(true);
stage.show();

clutter.main();
