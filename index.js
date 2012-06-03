var toolkit = module.exports = require('./build/Release/jsdx_toolkit');
toolkit.JTKML = require('./lib/jtkml');
toolkit.UIRender = require('./lib/ui-render');

/* JSDX Toolkit Object */
toolkit.Application.prototype.type = 'Application';
toolkit.Window.prototype.type = 'Window';
toolkit.Actor.prototype.type = 'Actor';
toolkit.Container.prototype.type = 'Container';
toolkit.Group.prototype.type = 'Group';
toolkit.Stage.prototype.type = 'Stage';
toolkit.Rectangle.prototype.type = 'Rectangle';
toolkit.Text.prototype.type = 'Text';
toolkit.Texture.prototype.type = 'Texture';
toolkit.State.prototype.type = 'State';
toolkit.GstVideoTexture.prototype.type = 'GstVideoTexture';
toolkit.Widget.Style.prototype.type = 'Style';
toolkit.Widget.Stack.prototype.type = 'Stack';
toolkit.Widget.BoxLayout.prototype.type = 'BoxLayout';
toolkit.Widget.Grid.prototype.type = 'Grid';
toolkit.Widget.Table.prototype.type = 'Table';
toolkit.Widget.Scroll.prototype.type = 'Scroll';
toolkit.Widget.Button.prototype.type = 'Button';
toolkit.Widget.Entry.prototype.type = 'Entry';
toolkit.Widget.Frame.prototype.type = 'Frame';
toolkit.Widget.Label.prototype.type = 'Label';
toolkit.Widget.Dialog.prototype.type = 'Dialog';
toolkit.Widget.ProgressBar.prototype.type = 'ProgressBar';
toolkit.Widget.Slider.prototype.type = 'Slider';
toolkit.Widget.Toggle.prototype.type = 'Toggle';
toolkit.Widget.Spinner.prototype.type = 'Spinner';
toolkit.Widget.Image.prototype.type = 'Image';
toolkit.Widget.Viewport.prototype.type = 'Viewport';
toolkit.Widget.ScrollView.prototype.type = 'ScrollView';
toolkit.Widget.KineticScrollView.prototype.type = 'KineticScrollView';

/* Internal functions */
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

/* Increasse reference to avoid GC to recycle */
toolkit.Group.prototype.add = add;
toolkit.Container.prototype.add = add;
toolkit.Stage.prototype.add = add;

function add() {
	if (arguments.length != 1)
		throw Error('add() syntax error: first argument is widget');

	var self = this;
	var args = Array.prototype.slice.call(arguments);
	var widget = args[0];

	widget.parent = this;

	if ('actor_list' in self)
		self.actor_list.push(widget);
	else
		self.actor_list = [ widget ];

	this._add.apply(this, args);

	/* If container belongs to specific application, child has the same one as well. */
	if ('application' in this) {

		/* TODO: Combine setApplication and getWidgetIdDict, DO NOT traverse widget tree twice. */
		setApplication(widget, this.application);

		/* Get all child of window which has id, to append them to list */
		getWidgetIdDict(widget, function(idDict) {

			/* Append to widget list of application */
			for (var id in idDict)
				self.application.widget[id] = idDict[id];
		});
	}
}

/* Window */
toolkit.Window.prototype.setChild = setChild;
toolkit.Window.prototype.add = setChild;

function setChild(child) {
	var self = this;

	child.parent = this;

	if ('actor_list' in self)
		self.actor_list.push(child);
	else
		self.actor_list = [ child ];

	this._setChild(child);

	/* If container belongs to specific application, child has the same one as well. */
	if ('application' in this) {

		/* TODO: Combine setApplication and getWidgetIdDict, DO NOT traverse widget tree twice. */
		setApplication(child, this.application);

		/* Get all child of window which has id, to append them to list */
		getWidgetIdDict(child, function(idDict) {

			/* Append to widget list of application */
			for (var id in idDict)
				self.application.widget[id] = idDict[id];
		});
	}
}

/* Widgets */
toolkit.Widget.BoxLayout.prototype.add = add;
toolkit.Widget.Stack.prototype.add = add;
toolkit.Widget.Table.prototype.add = add;
toolkit.Widget.KineticScrollView.prototype.add = add;
toolkit.Widget.ScrollView.prototype.add = add;
toolkit.Widget.Viewport.prototype.add = add;
toolkit.Widget.Button.prototype.add = add;
toolkit.Widget.Label.prototype.add = add;
toolkit.Widget.Entry.prototype.add = add;
toolkit.Widget.Frame.prototype.add = add;

toolkit.Widget.Dialog.prototype.add = add;
toolkit.Widget.Dialog.prototype.setTransientParent = setTransientParent;

function setTransientParent(_parent) {
	this.parent = _parent;

	if ('actor_list' in self)
		_parent.actor_list.push(this);
	else
		_parent.actor_list = [ this ];

	this._setTransientParent(_parent);

	if ('application' in _parent) {

		/* TODO: Combine setApplication and getWidgetIdDict, DO NOT traverse widget tree twice. */
		setApplication(this, _parent.application);

		/* Get all child of window which has id, to append them to list */
		getWidgetIdDict(this, function(idDict) {

			/* Append to widget list of application */
			for (var id in idDict)
				_parent.application.widget[id] = idDict[id];
		});
	}
}

/* Application */
toolkit.Application.prototype.add = function(window) {
	var self = this;

	if ('window_list' in self)
		self.window_list.push(window);
	else
		self.window_list = [ window ];

	this._add(window);

	window.parent = this;

	/* Initializing object to store childs */
	if (!('widget' in this))
		this.widget = {};

	/* Get all child of window which has id, to append them to list */
	getWidgetIdDict(window, function(idDict) {

		/* Append to widget list of application */
		for (var id in idDict)
			self.widget[id] = idDict[id];
	});

	/* set all child to belongs to this application */
	setApplication(window, this);
};

toolkit.Application.prototype.createWindow = function(callback) {
	var window = new toolkit.Window;
	this.add(window);

	if (callback)
		callback(window);
};

toolkit.Application.prototype.renderUI = function(structure) {
	
	var render = new toolkit.UIRender(toolkit, this);
	render.load(structure);
	render.generate(structure);

	/* Initializing object to store childs */
	if (!('widget' in this))
		this.widget = {};

	/* Append new widget to widget list of application */
	for (var id in render.ids)
		this.widget[id] = render.ids[id];

	/* Append window to window list */
	for (var index in render.widgets) {
		if (render.widgets[index].type == 'Window') {
			this.add(render.widgets[index]);
		}
	}
}

toolkit.Application.prototype.getWidgetById = function(id) {
	if (id in this.widget)
		return this.widget[id];

	throw Error('No such widget');
};
