var clutter = require('../index');

if (clutter.gst_init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('VideoTexture');
stage.resize(400, 300);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* Create a video texture */
var video = new clutter.GstVideoTexture;
video.setPosition(50, 50);
video.loadFile('jushelf-demo.ogv');
video.rotate(clutter.Y_AXIS, 30, 0, 0, 0);
stage.add(video);

video.play();

clutter.main();
