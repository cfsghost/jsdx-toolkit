var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'Button Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* Button Widget */
var button = new clutter.Widget.Button('I am a button');
console.log(button.label);
stage.add(button);

/* Button Widget */
var button2 = new clutter.Widget.Button();
button2.label = 'second button!'
button2.y = 50;
stage.add(button2);

clutter.main();
