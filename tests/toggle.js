var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'Toggle Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* Toggle Widget */
var toggle = new clutter.Widget.Toggle();
console.log(toggle.active);
stage.add(toggle);

/* Toggle Widget */
var toggle2 = new clutter.Widget.Toggle();
toggle2.active = false;
toggle2.y = 50;
stage.add(toggle2);

clutter.main();
