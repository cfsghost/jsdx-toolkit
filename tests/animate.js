var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Animate';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

var texture1 = new toolkit.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(0, 0);
texture1.opacity = 0;
stage.add(texture1);

texture1.animate(toolkit.EASE_IN_QUINT, 1000, {
	'x': 100,
	'y': 100,
	'opacity': 255
}, function() {
	texture1.animate(toolkit.EASE_IN_CUBIC, 1000, {
		'rotation-angle-x': 360
	}, { loop: true });
});

toolkit.main();
