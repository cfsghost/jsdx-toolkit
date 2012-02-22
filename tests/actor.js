var clutter = require('../build/Release/clutter');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

clutter.main();
