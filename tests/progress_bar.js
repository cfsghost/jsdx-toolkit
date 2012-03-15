var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'ProgressBar Widget';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(toolkit.EVENT_DESTROY, function() {
	toolkit.quit();
});
stage.show();

/* ProgressBar Widget */
var progressBar = new toolkit.Widget.ProgressBar;
progressBar.width = 200;
console.log(progressBar.progress);
stage.add(progressBar);

/* ProgressBar Widget */
var progressBar2 = new toolkit.Widget.ProgressBar;
progressBar2.y = 50;
progressBar2.width = 200;
progressBar2.progress = 0.6;
stage.add(progressBar2);

toolkit.main();
