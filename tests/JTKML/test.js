var toolkit = require('../../index');

var app = new toolkit.Application('Test Application');

var JTKML = new toolkit.JTKML;
JTKML.loadFile('test.jtkml', function(structure) {
	var uiObj = toolkit.UIRender(structure);

	app.renderUI(uiObj);
	uiObj.roots[0].show();
});
/*
app.createWindow(function(window) {
	window.on(toolkit.EVENT_DESTROY, function() {
		app.quit();
	});

	window.title = 'Test Window';
	window.hasToolbar = false;
	window.show();

});
*/
app.run();

