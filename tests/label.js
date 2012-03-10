var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'Label Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* Label Widget */
var label = new clutter.Widget.Label('Label!!');
console.log(label.text);
stage.add(label);

/* Label Widget */
var label2 = new clutter.Widget.Label();
label2.text = 'second Label!';
label2.y = 50;
stage.add(label2);

clutter.main();
