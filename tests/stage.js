var clutter = require('../build/Release/clutter');

clutter.useARGB(true);

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('This is a new stage');
console.log(stage.getTitle());
stage.useAlpha(true);
stage.setOpacity(100);
stage.resize(500, 500);
stage.setColor(255, 0, 0, 255);

stage.show();
stage.hide();
stage.showAll();

var rectangle = new clutter.Rectangle(0, 255, 255, 255);
rectangle.resize(100, 100);
rectangle.setPosition(100, 100);
stage.add(rectangle);

clutter.main();
