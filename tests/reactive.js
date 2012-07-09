var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Reactive';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

var texture1 = new toolkit.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.reactive = true;
stage.add(texture1);

toolkit.main();
