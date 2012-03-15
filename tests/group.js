var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'Group';
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(toolkit.EVENT_DESTROY, function() {
	toolkit.quit();
});
stage.show();

/* Create a group */
var group = new toolkit.Group;
stage.add(group);

var texture1 = new toolkit.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.rotate(toolkit.Y_AXIS, 45, 0, 0, 0);
group.add(texture1);

var texture2 = new toolkit.Texture;
texture2.loadFile('fred.jpg');
texture2.setPosition(100, 200);
texture2.rotate(90, toolkit.GRAVITY_CENTER);
group.add(texture2);

/* Move group */
group.setPosition(-100, -100);

toolkit.main();
