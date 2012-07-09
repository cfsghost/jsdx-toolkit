var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Label Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

/* Label Widget */
var label = new toolkit.Widget.Label('Label!!');
console.log(label.text);
stage.add(label);

/* Label Widget */
var label2 = new toolkit.Widget.Label();
label2.text = 'second Label!';
label2.y = 50;
stage.add(label2);

toolkit.main();
