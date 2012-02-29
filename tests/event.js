var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('Event');
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* Normal to load (default: sync) */
var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.opacity = 150;
texture1.reactive(true);
texture1.on(clutter.EVENT_ENTER, function(ev) {
	console.log('texture1: enter');

	return true;
});
texture1.on(clutter.EVENT_LEAVE, function(ev) {
	console.log('texture1: leave');

	return true;
});
texture1.on(clutter.EVENT_MOTION, function(ev, motion) {
	//console.log('texture1: motion');
	console.log('texture1: motion (' + motion.x + ', ' + motion.y + ') time=' + motion.time);

	return true;
});
stage.add(texture1);


/* Asynchronize */
var texture2 = new clutter.Texture;
texture2.setLoadAsync(true);
texture2.loadFile('fred.jpg');
texture2.setPosition(200, 200);
texture2.depth = -200;
texture2.reactive(true);
texture2.on(clutter.EVENT_CLICK, function(ev) {
	console.log('texture2: click');
});
stage.add(texture2);

/* Keep aspect ratio */
var texture3 = new clutter.Texture;
texture3.keepAspectRatio(true);
texture3.loadFile('fred.jpg');
texture3.width = 50;
texture3.setPosition(50, 50);
stage.add(texture3);

clutter.main();
