var clutter = require('../index');

clutter.useARGB(true);

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('This is a new stage');
console.log(stage.getTitle());
stage.useAlpha(true);
stage.opacity = 100;
stage.resize(500, 500);
stage.setColor(255, 0, 0, 255);
stage.setCursor(false);

stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});

stage.show();
stage.hide();
stage.showAll();

clutter.main();
