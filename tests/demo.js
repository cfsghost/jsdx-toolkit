var toolkit = require('../index');

toolkit.useARGB(true);

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'jsdx-toolkit Demo';
stage.useAlpha = true;
stage.opacity = 100;
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(toolkit.EVENT_DESTROY, function() {
	toolkit.quit();
});
stage.show();

var texture1 = new toolkit.Texture;
texture1.loadFile('fred.jpg', function() {
	console.log('Texture was loaded!');
});
texture1.setPosition(100, 50);
texture1.rotate(toolkit.Y_AXIS, 60, 0, 0, 0);
texture1.rotate(60, toolkit.GRAVITY_CENTER);
stage.add(texture1);

var texture2 = new toolkit.Texture;
texture2.loadFileSync('fred.jpg');
texture2.setPosition(200, 150);
texture2.rotate(30, toolkit.GRAVITY_CENTER);
texture2.reactive = true;
texture2.on(toolkit.EVENT_PRESS, function(ev) {
	console.log('Pressed texture2!');
});
texture2.on(toolkit.EVENT_RELEASE, function(ev) {
	console.log('Released texture2!');
});
texture2.on(toolkit.EVENT_CLICK, function(ev) {
	console.log('Clicked texture2!');
});
stage.add(texture2);

var text1 = new toolkit.Text('node-toolkit Rocks!');
text1.setFontName('Droid Sans bold 24');
text1.setPosition(50, 400);
text1.setColor(255, 255, 255, 255);
stage.add(text1);

toolkit.main();
