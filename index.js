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
	var self = this;

	self.hide();

	if (self.application) {
		/* Remove widget from widget list of application */
		if ('id' in self) {
			self.application.removeWidgetById(self.id);
		}
	}

	/* Destory childs */
	for (var index in self.actor_list) {
		var child = self.actor_list[index];

		child.destroy();
	}

	if (self.parent) {
		/* Take off this widget from parent */
		self.parent.remove.apply(self.parent, [ self ]);
	} else {
		/* Destroy this widget only */
		self._destroy.apply(self, []);
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
	var self = this;

	/* Destory specific widget */
	for (var index in self.actor_list) {
		if (self.actor_list[index] == widget) {
			/* remove and destory this widget */
			self._remove.apply(self, [ widget ]);

			delete self.actor_list[index];

			return;
		}
	}
}

/* Window */
toolkit.Window.prototype.__defineGetter__('id', function() {
	return this._id;
});
toolkit.Window.prototype.__defineSetter__('id', function(val) {
	this._id = val;

	/* Register this ID in application */
	if (this.application)
		this.application.widget[val] = this;
});
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
}

toolkit.Application.prototype.removeWidget = function(widget) {
	for (var id in self.widget) {
		if (self.widget[id] == widget) {
			delete self.widget[id];
			return;
		}
	}
};

toolkit.Application.prototype.removeWidgetById = function(id) {
	if (id in self.widget) {
		delete self.widget[id];
	}
};

toolkit.Application.prototype.getWidgetById = function(id) {
	if (id in this.widget)
		return this.widget[id];

	throw Error('No such widget');
};
