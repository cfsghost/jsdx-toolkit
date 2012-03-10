var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'Spinner Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* Spinner Widget */
var spinner = new clutter.Widget.Spinner();
console.log(spinner.animating);
stage.add(spinner);

/* Spinner Widget */
var spinner2 = new clutter.Widget.Spinner();
spinner2.animating = false;
spinner2.y = 50;
stage.add(spinner2);

clutter.main();
