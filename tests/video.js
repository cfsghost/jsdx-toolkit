var toolkit = require('../index');

if (toolkit.gst_init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'VideoTexture';
stage.resize(400, 300);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

/* Create a video texture */
var video = new toolkit.GstVideoTexture;
video.setPosition(50, 50);
video.loadFile('jushelf-demo.ogv');
video.rotate(toolkit.Y_AXIS, 30, 0, 0, 0);
stage.add(video);

video.play();

toolkit.main();
