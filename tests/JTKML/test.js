var toolkit = require('../../index');

var app = new toolkit.Application('Test Application');

var JTKML = new toolkit.JTKML;
JTKML.loadFile('test.jtkml', function(structure) {
	app.renderUI(structure);

	app.getWidgetById('window1').show();
	app.getWidgetById('window2').show();
});

app.run();

