var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Button Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

/* Button Widget */
var button = new toolkit.Widget.Button('I am a button');
console.log(button.label);
stage.add(button);

/* Button Widget */
var button2 = new toolkit.Widget.Button();
button2.label = 'second button!';
button2.y = 50;
stage.add(button2);

toolkit.main();
