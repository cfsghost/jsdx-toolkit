var clutter = require('../../index');

var app = new clutter.Application('Test Application');

app.createWindow(function(window) {
	window.on(clutter.EVENT_DESTROY, function() {
		app.quit();
	});

	window.title = 'Test Window';
	window.hasToolbar = false;
	window.rotation = 270;
	window.show();

	/* Label Widget */
	var label = new clutter.Widget.Label('Label!!');
	window.setChild(label);
});

app.run();

