var toolkit = require('../../index');

var app = new toolkit.Application('Test Application');

app.createWindow(function(window) {
	window.on(toolkit.EVENT_DESTROY, function() {
		app.quit();
	});

	window.title = 'Test Style';
	window.hasToolbar = false;
	window.show();

	var style = new toolkit.Widget.Style();
	style.loadFile('default.css');

	/* label Widget */
	var label = new toolkit.Widget.Label('Demo');
	label.className = 'red-label';
	label.applyStyle(style);
	window.setChild(label);
});

app.run();

