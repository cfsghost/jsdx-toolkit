var clutter = require('../../index');

var app = new clutter.Application('Test Application');

app.createWindow(function(window) {
	window.on(clutter.EVENT_DESTROY, function() {
		app.quit();
	});
	window.title = 'Test Window';
	window.hasToolbar = true;
	window.show();
});

app.run();

