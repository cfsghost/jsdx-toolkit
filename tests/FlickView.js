var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'FlickView Widget';
stage.resize(800, 600);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

var flickView = new clutter.FlickView;
flickView.width = 700;
flickView.height = 500;
flickView.allow_y_axis = false;
stage.add(flickView);

console.log(flickView.allow_x_axis);

/* Create something in FlickView */
var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.reactive(true);
flickView.add(texture1);

clutter.main();
