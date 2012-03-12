var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'Entry Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* Entry Widget */
var entry = new clutter.Widget.Entry('Entry!!');
console.log(entry.passwordChar);
console.log(entry.text);
stage.add(entry);

/* Entry Widget */
var entry2 = new clutter.Widget.Entry();
entry2.passwordChar = '*';
console.log(entry2.passwordChar);
entry2.text = 'second Entry!';
entry2.y = 50;
stage.add(entry2);

clutter.main();
