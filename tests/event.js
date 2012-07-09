var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Event';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

/* Normal to load (default: sync) */
var texture1 = new toolkit.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.opacity = 150;
texture1.reactive = true;
texture1.on('enter', function(ev) {
	console.log('texture1: enter');

	return true;
});
texture1.on('leave', function(ev) {
	console.log('texture1: leave');

	return true;
});
texture1.on('motion', function(ev, motion) {
	//console.log('texture1: motion');
	console.log('texture1: motion (' + motion.x + ', ' + motion.y + ') time=' + motion.time);

	return true;
});
stage.add(texture1);


/* Asynchronize */
var texture2 = new toolkit.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(200, 200);
texture2.depth = -200;
texture2.reactive = true;
texture2.on('click', function(ev) {
	console.log('texture2: click');
});
stage.add(texture2);

/* Keep aspect ratio */
var texture3 = new toolkit.Texture;
texture3.keepAspectRatio(true);
texture3.loadFile('fred.jpg');
texture3.width = 50;
texture3.setPosition(50, 50);
stage.add(texture3);

toolkit.main();
