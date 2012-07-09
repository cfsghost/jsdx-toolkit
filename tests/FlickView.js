var toolkit = require('../index');

if (toolkit.init() != toolkit.INIT_SUCCESS) {
	console.log("Failed to initialize toolkit.");
	process.exit();
}

/* Create a new stage */
var stage = new toolkit.Stage();
stage.title = 'FlickView Widget';
stage.resize(800, 600);
stage.setColor(0, 0, 0, 255);
stage.on('destroy', function() {
	toolkit.quit();
});
stage.show();

var flickView = new toolkit.FlickView;
flickView.width = 700;
flickView.height = 500;
flickView.allow_y_axis = false;
stage.add(flickView);

/* Create something in FlickView */
var texture1 = new toolkit.Texture;
texture1.loadFile('fred.jpg');
flickView.add(texture1);





/* Second stage to show page style mode */
var stage2 = new toolkit.Stage();
stage2.title = 'FlickView - Page Mode';
stage2.resize(800, 600);
stage2.setColor(0, 0, 0, 255);
stage2.show();

var flickView1 = new toolkit.FlickView;
flickView1.mode = toolkit.FlickView.MODE_PAGE_STYLE;
flickView1.width = 200;
flickView1.height = 500;
flickView1.allow_y_axis = false;
stage2.add(flickView1);

/* Page */
var page1 = new toolkit.Texture;
page1.loadFile('fred.jpg');
page1.width = 200;
page1.height = 200;
flickView1.add(page1);

var page2 = new toolkit.Texture;
page2.loadFile('fred.jpg');
page2.x = 200;
page2.width = 200;
page2.height = 200;
flickView1.add(page2);

toolkit.main();
