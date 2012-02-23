var clutter = require('../build/Release/clutter');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('Rotate');
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.show();

var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.rotate(clutter.Y_AXIS, 30, 0, 0, 0);
stage.add(texture1);


var texture2 = new clutter.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(100, 200);
texture2.rotate(30, clutter.GRAVITY_CENTER);
stage.add(texture2);

clutter.main();
