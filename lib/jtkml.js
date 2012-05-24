var fs = require('fs');
var util = require("util");

var JTKML = module.exports = function() {
	this.structure = [ 'JTKML', {}, [] ];
	this.parseIndex = 0;
	this.data = null;
};

JTKML.prototype.loadFile = function(filename, callback) {
	var self = this;

	fs.readFile(filename, function(err, data) {
		if (err)
			throw err;

		self.data = data.toString();

		var curPath = [ self.structure ];
		var curLevel = 0;
		var prevTag = null;
		while(self.parseIndex < self.data.length) {
			/* Skip */
			if (self.data.charAt(self.parseIndex) == '\n') {
				self.parseIndex++;
				continue;
			}

			/* Get level */
			var level = 0;
			for (; self.parseIndex < self.data.length; self.parseIndex++) {
				if (self.data.charAt(self.parseIndex) != '\t')
					break;

				level++;
			}

			/* Get tag object */
			var tag = self.parseLine();

			/* add to structure */
			if (level == curLevel) {
				curPath[curLevel][2].push(tag);
			} else if (level > curLevel) {
				curLevel++;
				curPath[curLevel] = prevTag;
				curPath[curLevel][2].push(tag);
			} else {
				curLevel = level;
				curPath[curLevel][2].push(tag);

				/* take off sub-node */
				curPath.length = curLevel + 1;
			}

			prevTag = tag;
		}

		if (callback)
			callback(self.structure);
	});
};

JTKML.prototype.parseAttrs = function() {
	var start = this.parseIndex;
	var attrsString = '';
	var attrs = {};

	while(true) {
		var c = this.data.charAt(this.parseIndex);

		if (c == ')') {
			this.parseIndex++;
			break;
		}

		this.parseIndex++;
	}

	attrsString = this.data.substring(start + 1, this.parseIndex - 1);

	/* Get all attributes */
	var parts = attrsString.split(',');
	for (var index in parts) {
		var part = parts[index].split('=');
		var attrName = part[0].replace(/(^\s*)|(\s*$)/g, "");
		var attrValue = part[1].replace(/(^\s*)|(\s*$)/g, "");

		if (attrValue.charAt(0) == '\'' && attrValue.charAt(attrValue.length-1) == '\'') {
			attrValue = attrValue.slice(1, attrValue.length-1);
		} else if (attrValue.charAt(0) == '\"' && attrValue.charAt(attrValue.length-1) == '\"') {
			attrValue = attrValue.slice(1, attrValue.length-1);
		} else {
			throw Error('Syntax Error: Attribute value needs to be wraped in `\'` or `\"`');
		}

		attrs[attrName] = attrValue;
	}

	return attrs;
};

JTKML.prototype.parseLine = function() {
	var attrs = {};

	/* Get tag */
	var tag = [];
	var start = this.parseIndex;
	var end = -1;
	while(this.parseIndex < this.data.length) {
		var c = this.data.charAt(this.parseIndex);

		if (c == '(') {
			if (end == -1)
				throw Error('No tag name');

			attrs = this.parseAttrs();

			continue;
		} else if (c == '\n') {
			if (end == -1)
				throw Error('No tag name');

			tag[0] = this.data.substring(start, end);
			tag[1] = attrs;
			tag[2] = [];

			this.parseIndex++;

			break;
		}

		this.parseIndex++;

		end = this.parseIndex;
	}

	return tag;
};
