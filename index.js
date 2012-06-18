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

/* Destroy */
//toolkit.Application.prototype.destroy = destroy;
toolkit.Window.prototype.destroy = destroy;
toolkit.Actor.prototype.destroy = destroy;
toolkit.Container.prototype.destroy = destroy;
toolkit.Group.prototype.destroy = destroy;
toolkit.Stage.prototype.destroy = destroy;
toolkit.Rectangle.prototype.destroy = destroy;
toolkit.Text.prototype.destroy = destroy;
toolkit.Texture.prototype.destroy = destroy;
toolkit.State.prototype.destroy = destroy;
toolkit.GstVideoTexture.prototype.destroy = destroy;
toolkit.Widget.Style.prototype.destroy = destroy;
toolkit.Widget.Stack.prototype.destroy = destroy;
toolkit.Widget.BoxLayout.prototype.destroy = destroy;
toolkit.Widget.Grid.prototype.destroy = destroy;
toolkit.Widget.Table.prototype.destroy = destroy;
toolkit.Widget.Scroll.prototype.destroy = destroy;
toolkit.Widget.Button.prototype.destroy = destroy;
toolkit.Widget.Entry.prototype.destroy = destroy;
toolkit.Widget.Frame.prototype.destroy = destroy;
toolkit.Widget.Label.prototype.destroy = destroy;
toolkit.Widget.Dialog.prototype.destroy = destroy;
toolkit.Widget.ProgressBar.prototype.destroy = destroy;
toolkit.Widget.Slider.prototype.destroy = destroy;
toolkit.Widget.Toggle.prototype.destroy = destroy;
toolkit.Widget.Spinner.prototype.destroy = destroy;
toolkit.Widget.Image.prototype.destroy = destroy;
toolkit.Widget.Viewport.prototype.destroy = destroy;
toolkit.Widget.ScrollView.prototype.destroy = destroy;
toolkit.Widget.KineticScrollView.prototype.destroy = destroy;

function destroy() {
	this.hide();

	if (this.application) {
		/* Remove widget from widget list of application */
		if ('id' in this) {
			this.application.removeWidgetById(this.id);
		}
	}

	/* Destory childs */
	for (var index in this.actor_list) {
		var child = this.actor_list[index];

		child.destroy();
	}

	if (this.parent) {
		/* Take off this widget from parent */
		this.parent.remove.apply(this.parent, [ this ]);
	} else {
		/* Destroy this widget only */
		this._destroy.apply(this, []);
	}
}

/* Increasse reference to avoid GC to recycle */
toolkit.Group.prototype.add = add;
toolkit.Group.prototype.remove = remove;
toolkit.Container.prototype.add = add;
toolkit.Container.prototype.remove = remove;
toolkit.Stage.prototype.add = add;
toolkit.Stage.prototype.remove = remove;

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

	/* Call internal method in next time, to avoid that process was confused cause crashing */
	process.nextTick(function() {
		self._add.apply(self, args);
	});

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

function remove(widget) {

	/* Destory specific widget */
	for (var index in this.actor_list) {
		if (this.actor_list[index] == widget) {
			/* remove and destory this widget */
			this._remove.apply(this, [ widget ]);

			delete this.actor_list[index];

			return;
		}
	}
}

/* Unique ID Handler */
function idGetter() {
	if ('_id' in this)
		return this._id;
	else
		return undefined;
};

function idSetter(val) {

	/* Register this ID in application */
	if (this.application) {

		/* Initializing object to store childs */
		if (!('widget' in this.application))
			this.application.widget = {};

		/* Has ID already */
		if ('_id' in this) {

			/* Unregister old */
			if (this._id in this.application)
				delete this.application.widget[this._id];
		}

		this.application.widget[val] = this;
	}

	this._id = val;
};

toolkit.Window.prototype.__defineGetter__('id', idGetter);
toolkit.Window.prototype.__defineSetter__('id', idSetter);
toolkit.Actor.prototype.__defineGetter__('id', idGetter);
toolkit.Actor.prototype.__defineSetter__('id', idSetter);
toolkit.Container.prototype.__defineGetter__('id', idGetter);
toolkit.Container.prototype.__defineSetter__('id', idSetter);
toolkit.Group.prototype.__defineGetter__('id', idGetter);
toolkit.Group.prototype.__defineSetter__('id', idSetter);
toolkit.Stage.prototype.__defineGetter__('id', idGetter);
toolkit.Stage.prototype.__defineSetter__('id', idSetter);
toolkit.Rectangle.prototype.__defineGetter__('id', idGetter);
toolkit.Rectangle.prototype.__defineSetter__('id', idSetter);
toolkit.Text.prototype.__defineGetter__('id', idGetter);
toolkit.Text.prototype.__defineSetter__('id', idSetter);
toolkit.Texture.prototype.__defineGetter__('id', idGetter);
toolkit.Texture.prototype.__defineSetter__('id', idSetter);
toolkit.State.prototype.__defineGetter__('id', idGetter);
toolkit.State.prototype.__defineSetter__('id', idSetter);
toolkit.GstVideoTexture.prototype.__defineGetter__('id', idGetter);
toolkit.GstVideoTexture.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Style.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Style.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Stack.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Stack.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.BoxLayout.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.BoxLayout.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Grid.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Grid.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Table.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Table.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Scroll.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Scroll.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Button.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Button.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Entry.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Entry.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Frame.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Frame.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Label.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Label.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Dialog.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Dialog.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.ProgressBar.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.ProgressBar.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Slider.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Slider.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Toggle.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Toggle.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Spinner.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Spinner.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Image.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Image.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.Viewport.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.Viewport.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.ScrollView.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.ScrollView.prototype.__defineSetter__('id', idSetter);
toolkit.Widget.KineticScrollView.prototype.__defineGetter__('id', idGetter);
toolkit.Widget.KineticScrollView.prototype.__defineSetter__('id', idSetter);

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
toolkit.Widget.BoxLayout.prototype.remove = remove;
toolkit.Widget.Stack.prototype.add = add;
toolkit.Widget.Stack.prototype.remove = remove;
toolkit.Widget.Table.prototype.add = add;
toolkit.Widget.Table.prototype.remove = remove;
toolkit.Widget.KineticScrollView.prototype.add = add;
toolkit.Widget.KineticScrollView.prototype.remove = remove;
toolkit.Widget.ScrollView.prototype.add = add;
toolkit.Widget.ScrollView.prototype.remove = remove;
toolkit.Widget.Viewport.prototype.add = add;
toolkit.Widget.Viewport.prototype.remove = remove;
toolkit.Widget.Button.prototype.add = add;
toolkit.Widget.Button.prototype.remove = remove;
toolkit.Widget.Label.prototype.add = add;
toolkit.Widget.Label.prototype.remove = remove;
toolkit.Widget.Entry.prototype.add = add;
toolkit.Widget.Entry.prototype.remove = remove;
toolkit.Widget.Frame.prototype.add = add;
toolkit.Widget.Frame.prototype.remove = remove;

toolkit.Widget.Dialog.prototype.add = add;
toolkit.Widget.Dialog.prototype.remove = remove;
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

	/* Call internal method in next time, to avoid that process was confused cause crashing */
	process.nextTick(function() {
		self._add(window);
	});

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
};

toolkit.Application.prototype.removeWidget = function(widget) {
	var self = this;

	for (var id in self.widget) {
		if (self.widget[id] == widget) {
			delete self.widget[id];
			return;
		}
	}
};

toolkit.Application.prototype.removeWidgetById = function(id) {
	var self = this;

	if (id in self.widget) {
		delete self.widget[id];
	}
};

toolkit.Application.prototype.getWidgetById = function(id) {
	if (id in this.widget)
		return this.widget[id];

	throw Error('No such widget');
};

/* BoxLayout */
toolkit.Widget.BoxLayout.prototype.setExpand = function() {
	var self = this;
	var args = Array.prototype.slice.call(arguments);

	process.nextTick(function() {
		self._setExpand.apply(self, args);
	});
};
