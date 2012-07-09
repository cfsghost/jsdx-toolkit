var toolkit = require('../index');

toolkit.useARGB(true);

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'This is a new stage';
console.log(stage.title);
stage.useAlpha = true;
stage.opacity = 100;
stage.resize(500, 500);
stage.setColor(255, 0, 0, 255);
stage.setCursor(false);

stage.on('destroy', function() {
	toolkit.quit();
});

stage.show();
stage.hide();
stage.showAll();

toolkit.main();
