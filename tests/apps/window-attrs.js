var toolkit = require('../../index');

var app = new toolkit.Application('Test Application');

app.createWindow(function(window) {
	window.on(toolkit.EVENT_DESTROY, function() {
		app.quit();
	});

	window.title = 'Test Window';
	window.hasDecorator = false;
	window.hasToolbar = false;
	window.rotation = 270;
	window.show();

	/* Label Widget */
	var label = new toolkit.Widget.Label('Label!!');
	window.setChild(label);
});

app.run();

