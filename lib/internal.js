
/* Internal functions */

module.exports = {
	traverseWidgets: traverseWidgets,
	getWidgetIdDict: getWidgetIdDict,
	setApplication: setApplication
};

function traverseWidgets(widget, handler) {

	handler(widget);

	if ('actor_list' in widget) {
		for (var index in widget.actor_list) {
			traverseWidgets(widget.actor_list[index], handler);
		}
	}
}

function getWidgetIdDict(widget, callback) {
	var idDict = {};

	traverseWidgets(widget, function(w) {
		if ('id' in w) {
			idDict[w.id] = w;
		}
	});

	process.nextTick(function() {
		callback(idDict);
	});
}

function setApplication(widget, app) {

	/* Set all widgets */
	traverseWidgets(widget, function(w) {
		w.application = app;
	});
}
