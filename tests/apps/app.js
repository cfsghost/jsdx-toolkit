var toolkit = require('../../index');

var app = new toolkit.Application('Test Application');

app.createWindow(function(window) {
	window.on('destroy', function() {
		app.quit();
	});

	window.title = 'Test Window';
	window.hasToolbar = false;
	window.show();
});

app.run();

