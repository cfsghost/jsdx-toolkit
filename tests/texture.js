var clutter = require('../index');

if (clutter.init() != clutter.INIT_SUCCESS) {
	console.log("Failed to initialize clutter.");
	process.exit();
}

/* Create a new stage */
var stage = new clutter.Stage();
stage.setTitle('Texture');
stage.resize(500, 500);
stage.setColor(0, 0, 0, 255);
stage.on(clutter.EVENT_DESTROY, function() {
	clutter.quit();
});
stage.show();

/* Normal to load (default: sync) */
var texture1 = new clutter.Texture;
texture1.loadFile('fred.jpg');
texture1.setPosition(100, 100);
texture1.opacity = 150;
//texture1.effect(clutter.EFFECT_BLUR);
texture1.effect(clutter.EFFECT_PAGE_TURN, { period: 0.5, angle: 30, radius: 30 });
stage.add(texture1);

console.log('texture1 opacity = ' + texture1.opacity);

/* Asynchronize */
var texture2 = new clutter.Texture;
texture2.setLoadAsync(true);
texture2.loadFile('fred.jpg');
texture2.setPosition(200, 200);
texture2.setDepth(-200);
texture2.effect(clutter.EFFECT_COLORIZE, [ 255, 255, 255, 255 ]);
stage.add(texture2);

console.log('texture2 getLoadAsync() = ' + texture2.getLoadAsync());
console.log('texture2 getDepth() = ' + texture2.getDepth());

/* Keep aspect ratio */
var texture3 = new clutter.Texture;
texture3.keepAspectRatio(true);
texture3.loadFile('fred.jpg');
texture3.width = 50;
texture3.setPosition(50, 50);
texture3.effect(clutter.EFFECT_DESATURATE, 0.5);
stage.add(texture3);

console.log('texture3 keepAspectRatio() = ' + texture3.keepAspectRatio());
console.log('texture3 (x, y) = (' + texture3.x + ', ' + texture3.y + ')');
console.log('texture3 (w, h) = (' + texture3.width + ', ' + texture3.height + ')');

clutter.main();
