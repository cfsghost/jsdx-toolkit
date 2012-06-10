var UIRender = module.exports = function(toolkit, application) {
	this.application = application;
	this.toolkit = toolkit;
	this.structure = null;
	this.ids = {};
	this.widgets = [];
};

UIRender.prototype.load = function(structure) {
	if (structure[0] != 'JTKML')
		throw Error('It\'s not a JTKML structure object');

	this.structure = structure;
};

UIRender.prototype.generate = function() {
	if (!this.structure)
		throw Error('No structure object to render UI');

	var self = this;
	var ids = {};

	function _create(tag) {
		var tagName = tag[0];
		var widget = null;

		/* Create widget */
		if (tagName in self.toolkit)
			widget = new self.toolkit[tagName];
		else if (tagName in self.toolkit.Widget)
			widget = new self.toolkit.Widget[tagName];
		else
			throw Error('No such widget \'' + tagName + '\'');

		if (widget.Type == 'window')
			self.application.add(widget);
		else
			widget.application = self.application;

		/* Attributes */
		for (var key in tag[1]) {
			widget[key] = tag[1][key];
		}

		/* Has ID attribute, adding to dict */
		if ('id' in widget)
			ids[widget['id']] = widget;

		/* Create sub widgets */
		var widgets = _traverse(tag);
		for (var index in widgets)
			widget.add(widgets[index]);

		return widget;
	}

	function _traverse(structure) {
		var widgets = [];
		var node = structure[2];

		/* Traverse structure to create all components */
		for (var index in structure[2]) {
			var tag = node[index];
			var w = _create(tag);

			widgets.push(w);
		}

		return widgets;
	}

	this.widgets = _traverse(this.structure);
	this.ids = ids;

};
