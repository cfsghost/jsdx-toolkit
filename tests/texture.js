var clutter = require('../build/Release/clutter');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('Text');
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.show();

var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
stage.add(texture1);

clutter.main();
