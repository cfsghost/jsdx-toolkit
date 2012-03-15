var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Spinner Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(toolkit.EVENT_DESTROY, function() {
	toolkit.quit();
});
stage.show();

/* Spinner Widget */
var spinner = new toolkit.Widget.Spinner();
console.log(spinner.animating);
stage.add(spinner);

/* Spinner Widget */
var spinner2 = new toolkit.Widget.Spinner();
spinner2.animating = false;
spinner2.y = 50;
stage.add(spinner2);

toolkit.main();
