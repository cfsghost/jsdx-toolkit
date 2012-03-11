node-clutter
---
Node.js binding for Clutter Toolkit. It aims to provide a node.js module for 3D user interface development.


License
-
Licensed under the MIT License

Get Started
-
It's easy to write a new application in Node.js:
  
    var clutter = require('clutter');
    
    /* Create a new application */
    var app = new clutter.Application('Clutter Application');
    
    /* Create a new window */
    app.createWindow(function(window) {
    
      /* Quit current application when this window is destroyed */
      window.on(clutter.EVENT_DESTORY, function() {
        app.quit();
      });

      window.title = 'Application Window';
      window.hasToolbar = true;
      window.show();
    });
    
    app.run();


Authors
-
Copyright(c) 2012 Fred Chien <<fred@mandice.com>>

Copyright
-
Copyright(c) 2012 Mandice Company.
(http://www.mandice.com/)