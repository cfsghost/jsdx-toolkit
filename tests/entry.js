var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Entry Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

/* Entry Widget */
var entry = new toolkit.Widget.Entry('Entry!!');
console.log(entry.passwordChar);
console.log(entry.text);
stage.add(entry);

/* Entry Widget */
var entry2 = new toolkit.Widget.Entry();
entry2.passwordChar = '*';
console.log(entry2.passwordChar);
entry2.text = 'second Entry!';
entry2.y = 50;
stage.add(entry2);

toolkit.main();
