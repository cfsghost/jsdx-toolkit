var clutter = require('../../index');

var app = new clutter.Application('Test Application');
app.createWindow(function(window) {
	window.title = 'Test Window';
	window.hasToolbar = true;
	window.show();
});

clutter.main();
