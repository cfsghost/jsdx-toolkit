var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.title = 'FlickView Widget';
stage.resize(800, 600);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

var flickView = new clutter.FlickView;
flickView.width = 700;
flickView.height = 500;
flickView.allow_y_axis = false;
stage.add(flickView);

/* Create something in FlickView */
var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
flickView.add(texture1);





/* Second stage to show page style mode */
var stage2 = new clutter.Stage();
stage2.title = 'FlickView - Page Mode';
stage2.resize(800, 600);
stage2.setColor(0, 0, 0, 255);
stage2.show();

var flickView1 = new clutter.FlickView;
flickView1.mode = clutter.FlickView.MODE_PAGE_STYLE;
flickView1.width = 200;
flickView1.height = 500;
flickView1.allow_y_axis = false;
stage2.add(flickView1);

/* Page */
var page1 = new clutter.Texture;
page1.loadFile('fred.jpg');
page1.width = 200;
page1.height = 200;
flickView1.add(page1);

var page2 = new clutter.Texture;
page2.loadFile('fred.jpg');
page2.x = 200;
page2.width = 200;
page2.height = 200;
flickView1.add(page2);

clutter.main();
