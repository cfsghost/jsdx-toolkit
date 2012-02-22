var clutter = require('../build/Release/clutter');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('Text');
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.show();

var text1 = new clutter.Text('WHITE TEXT');
text1.setPosition(100, 100);
text1.setColor(255, 255, 255, 255);
console.log(text1.getText());
text1.setText(text1.getText() + '!!!');
console.log('Text1\'s Color is ');
console.log(text1.getColor());
stage.add(text1);

clutter.main();
