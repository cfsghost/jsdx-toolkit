var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'ProgressBar Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* ProgressBar Widget */
var progressBar = new clutter.Widget.ProgressBar;
progressBar.width = 200;
console.log(progressBar.progress);
stage.add(progressBar);

/* ProgressBar Widget */
var progressBar2 = new clutter.Widget.ProgressBar;
progressBar2.y = 50;
progressBar2.width = 200;
progressBar2.progress = 0.6;
stage.add(progressBar2);

clutter.main();
