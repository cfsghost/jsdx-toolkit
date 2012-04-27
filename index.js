var toolkit = module.exports = require('./build/Release/jsdx_toolkit');

/* Increasse reference to avoid GC to recycle */
toolkit.Group.prototype.actor_list = [];
toolkit.Group.prototype.add = add;
toolkit.Container.prototype.actor_list = [];
toolkit.Container.prototype.add = add;
toolkit.Stage.prototype.actor_list = [];
toolkit.Stage.prototype.add = add;

function add() {
	var args = Array.prototype.slice.call(arguments);

	this.actor_list.push(args[0]);
	this._add.apply(this, args);
}

/* Window */
toolkit.Window.prototype.actor_list = [];
toolkit.Window.prototype.setChild = setChild;

function setChild(child) {
	this.actor_list.push(child);
	this._setChild(child);
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
	_parent.actor_list.push(this);
	this._setTransientParent(_parent);
}

/* Application */
toolkit.Application.prototype.window_list = [];
toolkit.Application.prototype.add = function(window) {
	toolkit.Application.prototype.window_list.push(window);
	this._add(window);
};

toolkit.Application.prototype.createWindow = function(callback) {
	var window = new toolkit.Window();
	this.add(window);

	if (callback)
		callback(window);
};
