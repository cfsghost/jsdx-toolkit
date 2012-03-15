var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Rectangle';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(toolkit.EVENT_DESTROY, function() {
	toolkit.quit();
});
stage.show();

var rectangle1 = new toolkit.Rectangle;
rectangle1.resize(100, 100);
rectangle1.setPosition(100, 100);
rectangle1.setColor(0, 0, 255, 255);
rectangle1.setBorder(2);
rectangle1.setBorderColor(255, 255, 255, 255);
console.log('Rectangle1\'s Border Width is ' + rectangle1.getBorder());
console.log('Rectangle1\'s Border Color is ');
console.log(rectangle1.getBorderColor());
stage.add(rectangle1);

var rectangle2 = new toolkit.Rectangle(0, 255, 255, 255);
console.log('Rectangle2\'s Color is ');
console.log(rectangle2.getColor());
rectangle2.resize(100, 100);
rectangle2.setPosition(200, 200);
stage.add(rectangle2);


toolkit.main();
