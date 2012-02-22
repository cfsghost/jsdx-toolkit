var clutter = require('../build/Release/clutter');

clutter.useARGB(true);

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('This is a new stage');
stage.useAlpha(true);
stage.setOpacity(100);
stage.resize(500, 500);
stage.setPosition(100, 100);

stage.show();
stage.hide();
stage.showAll();

clutter.main();
