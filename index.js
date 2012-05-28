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

/* Parent of Object */
toolkit.Application.prototype.parent = null;
toolkit.Window.prototype.parent = null;
toolkit.Actor.prototype.parent = null;
toolkit.Container.prototype.parent = null;
toolkit.Group.prototype.parent = null;
toolkit.Stage.prototype.parent = null;
toolkit.Rectangle.prototype.parent = null;
toolkit.Text.prototype.parent = null;
toolkit.Texture.prototype.parent = null;
toolkit.State.prototype.parent = null;
toolkit.GstVideoTexture.prototype.parent = null;
toolkit.Widget.Style.prototype.parent = null;
toolkit.Widget.BoxLayout.prototype.parent = null;
toolkit.Widget.Grid.prototype.parent = null;
toolkit.Widget.Table.prototype.parent = null;
toolkit.Widget.Scroll.prototype.parent = null;
toolkit.Widget.Button.prototype.parent = null;
toolkit.Widget.Entry.prototype.parent = null;
toolkit.Widget.Frame.prototype.parent = null;
toolkit.Widget.Label.prototype.parent = null;
toolkit.Widget.Dialog.prototype.parent = null;
toolkit.Widget.ProgressBar.prototype.parent = null;
toolkit.Widget.Slider.prototype.parent = null;
toolkit.Widget.Toggle.prototype.parent = null;
toolkit.Widget.Spinner.prototype.parent = null;
toolkit.Widget.Image.prototype.parent = null;
toolkit.Widget.Viewport.prototype.parent = null;
toolkit.Widget.ScrollView.prototype.parent = null;
toolkit.Widget.KineticScrollView.prototype.parent = null;

/* Application object */
toolkit.Application.prototype.application = null;
toolkit.Window.prototype.application = null;
toolkit.Actor.prototype.application = null;
toolkit.Container.prototype.application = null;
toolkit.Group.prototype.application = null;
toolkit.Stage.prototype.application = null;
toolkit.Rectangle.prototype.application = null;
toolkit.Text.prototype.application = null;
toolkit.Texture.prototype.application = null;
toolkit.State.prototype.application = null;
toolkit.GstVideoTexture.prototype.application = null;
toolkit.Widget.Style.prototype.application = null;
toolkit.Widget.BoxLayout.prototype.application = null;
toolkit.Widget.Grid.prototype.application = null;
toolkit.Widget.Table.prototype.application = null;
toolkit.Widget.Scroll.prototype.application = null;
toolkit.Widget.Button.prototype.application = null;
toolkit.Widget.Entry.prototype.application = null;
toolkit.Widget.Frame.prototype.application = null;
toolkit.Widget.Label.prototype.application = null;
toolkit.Widget.Dialog.prototype.application = null;
toolkit.Widget.ProgressBar.prototype.application = null;
toolkit.Widget.Slider.prototype.application = null;
toolkit.Widget.Toggle.prototype.application = null;
toolkit.Widget.Spinner.prototype.application = null;
toolkit.Widget.Image.prototype.application = null;
toolkit.Widget.Viewport.prototype.application = null;
toolkit.Widget.ScrollView.prototype.application = null;
toolkit.Widget.KineticScrollView.prototype.application = null;

/* Internal functions */
function traverseWidgets(widget, handler) {

	handler(widget);

	for (var index in widget.actor_list) {
		traverseWidgets(widget.actor_list[index], handler);
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
toolkit.Group.prototype.actor_list = [];
toolkit.Group.prototype.add = add;
toolkit.Container.prototype.actor_list = [];
toolkit.Container.prototype.add = add;
toolkit.Stage.prototype.actor_list = [];
toolkit.Stage.prototype.add = add;

function add() {
	if (arguments.length != 1)
		throw Error('add() syntax error: first argument is widget');

	var self = this;
	var args = Array.prototype.slice.call(arguments);
	var widget = args[0];

	widget.parent = this;

	this.actor_list.push(widget);
	this._add.apply(this, args);

	/* If container belongs to specific application, child has the same one as well. */
	if (this.application) {

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
toolkit.Window.prototype.actor_list = [];
toolkit.Window.prototype.setChild = setChild;
toolkit.Window.prototype.add = setChild;

function setChild(child) {
	var self = this;

	child.parent = this;

	this.actor_list.push(child);
	this._setChild(child);

	/* If container belongs to specific application, child has the same one as well. */
	if (this.application) {

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
toolkit.Widget.BoxLayout.prototype.actor_list = [];
toolkit.Widget.BoxLayout.prototype.add = add;
toolkit.Widget.Table.prototype.actor_list = [];
toolkit.Widget.Table.prototype.add = add;
toolkit.Widget.KineticScrollView.prototype.actor_list = [];
toolkit.Widget.KineticScrollView.prototype.add = add;
toolkit.Widget.ScrollView.prototype.actor_list = [];
toolkit.Widget.ScrollView.prototype.add = add;
toolkit.Widget.Viewport.prototype.actor_list = [];
toolkit.Widget.Viewport.prototype.add = add;
toolkit.Widget.Button.prototype.actor_list = [];
toolkit.Widget.Button.prototype.add = add;
toolkit.Widget.Label.prototype.actor_list = [];
toolkit.Widget.Label.prototype.add = add;
toolkit.Widget.Entry.prototype.actor_list = [];
toolkit.Widget.Entry.prototype.add = add;
toolkit.Widget.Frame.prototype.actor_list = [];
toolkit.Widget.Frame.prototype.add = add;

toolkit.Widget.Dialog.prototype.actor_list = [];
toolkit.Widget.Dialog.prototype.add = add;
toolkit.Widget.Dialog.prototype.setTransientParent = setTransientParent;

function setTransientParent(_parent) {
	this.parent = _parent;

	_parent.actor_list.push(this);
	this._setTransientParent(_parent);

	if (_parent.application) {

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
toolkit.Application.prototype.window_list = [];
toolkit.Application.prototype.widget = {};
toolkit.Application.prototype.add = function(window) {
	var self = this;

	toolkit.Application.prototype.window_list.push(window);
	this._add(window);

	window.parent = this;

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
