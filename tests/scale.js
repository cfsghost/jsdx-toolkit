var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Scale';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(toolkit.EVENT_DESTROY, function() {
	toolkit.quit();
});
stage.show();

var texture1 = new toolkit.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.scale(0.5, 0.5);
stage.add(texture1);

var scale = texture1.scale();
console.log('texture1 scale_x = ' + scale.scale_x)
console.log('texture1 scale_y = ' + scale.scale_y)

var texture2 = new toolkit.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(100, 200);
texture2.scale(0.5, 0.5, toolkit.GRAVITY_CENTER);
stage.add(texture2);

toolkit.main();
