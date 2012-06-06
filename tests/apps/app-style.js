var toolkit = require('../../index');

var app = new toolkit.Application('Test Application');

app.loadStyleFile('default.css');
app.createWindow(function(window) {
	window.on(toolkit.EVENT_DESTROY, function() {
		app.quit();
	});

	window.title = 'Test Style';
	window.hasToolbar = false;
	window.show();

	/* label Widget */
	var label = new toolkit.Widget.Label('Demo');
	label.className = 'red-label';
	window.setChild(label);
});

app.run();

