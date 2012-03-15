var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Text';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(toolkit.EVENT_DESTROY, function() {
	toolkit.quit();
});
stage.show();

var text1 = new toolkit.Text('WHITE TEXT');
text1.setPosition(100, 100);
text1.setColor(255, 255, 255, 255);
console.log(text1.getText());
text1.setText(text1.getText() + '!!!');
console.log('Text1\'s Color is ');
console.log(text1.getColor());
stage.add(text1);

var text2 = new toolkit.Text('Test Font');
text2.setFontName('Droid Sans 40');
console.log(text2.getFontName());
text2.setPosition(200, 200);
text2.setColor(255, 0, 0, 255);
stage.add(text2);

var text3 = new toolkit.Text('Bold Font');
text3.setFontName('Droid Sans bold 40');
console.log(text3.getFontName());
text3.setPosition(200, 300);
text3.setColor(200, 200, 0, 255);
stage.add(text3);

toolkit.main();
