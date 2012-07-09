var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Image Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

/* Image Widget */
var image = new toolkit.Widget.Image();
image.loadFile('fred.jpg', function() {
	console.log('Image was loaded!');
});
stage.add(image);

toolkit.main();
