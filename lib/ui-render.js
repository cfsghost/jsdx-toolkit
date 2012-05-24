var toolkit = require('../build/Release/jsdx_toolkit');

module.exports = function(structure) {

	var compCreator = new ComponentCreator;

	compCreator.load(structure);

	return compCreator.uiObj;
};

var ComponentCreator = function() {
	this.uiObj = {
		widgets: [],
		roots: []
	};
};

ComponentCreator.prototype.load = function(structure) {
	if (structure[0] != 'JTKML')
		throw Error('It\'s not a JTKML structure object');

	for (var index in structure[2]) {
		var w = this.createComponent(structure[2][index]);

		if (w.type == 'Window') {
			var winCreator = new WindowCreator;
		}

		this.uiObj.roots.push(w);
	}
};

var WindowCreator = function() {
	this.uiObj = {
		widgets: [],
		roots: []
	};
};

//function createComponent(tag) {
ComponentCreator.prototype.createComponent = function(tag) {
	var widget = null;

	if (tag[0] in toolkit)
		widget = new toolkit[tag[0]];
	else if (tag[0] in toolkit.Widget)
		widget = new toolkit.Widget[tag[0]];
	else
		throw Error('No such widget \'' + tag[0] + '\'');

	/* Attributes */
	for (var key in tag[1]) {
		widget[key] = tag[1][key];
	}

	/* Add to widget list */
	this.uiObj.widgets.push(w);

	/* Create sub widgets */
	for (var index in tag[2]) {
		var w = this.createComponent(tag[2][index]);
		widget.add(w);
	}

	return widget;
};
