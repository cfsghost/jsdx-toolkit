var clutter = require('../index');

clutter.useARGB(true);

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('node-clutter Demo');
stage.useAlpha(true);
stage.setOpacity(100);
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.show();

var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 50);
texture1.rotate(clutter.Y_AXIS, 60, 0, 0, 0);
texture1.rotate(60, clutter.GRAVITY_CENTER);
stage.add(texture1);

var texture2 = new clutter.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(200, 150);
texture2.rotate(30, clutter.GRAVITY_CENTER);
texture2.reactive(true);
texture2.on(clutter.EVENT_CLICK, function(ev) {
	console.log('Clicked texture2!');
});
stage.add(texture2);

var text1 = new clutter.Text('node-clutter Rocks!');
text1.setFontName('Droid Sans bold 24');
text1.setPosition(50, 400);
text1.setColor(255, 255, 255, 255);
stage.add(text1);

clutter.main();
