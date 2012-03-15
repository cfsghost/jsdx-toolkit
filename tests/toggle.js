var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Toggle Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(toolkit.EVENT_DESTROY, function() {
	toolkit.quit();
});
stage.show();

/* Toggle Widget */
var toggle = new toolkit.Widget.Toggle();
console.log(toggle.active);
stage.add(toggle);

/* Toggle Widget */
var toggle2 = new toolkit.Widget.Toggle();
toggle2.active = false;
toggle2.y = 50;
stage.add(toggle2);

toolkit.main();
