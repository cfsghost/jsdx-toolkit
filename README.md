jsdx-toolkit
---
A graphical toolkit on JSDX framework. It aims to provide a node.js module for 3D user interface development.

Installation
---
With Steps, you can use git version of JSDX toolkit:

1. Download source code from github

        git clone https://cfsghost@github.com/cfsghost/jsdx-toolkit.git

2. Change directory into folder

        cd jsdx-toolkit

3. Configure and compile it

        node-waf configure build

Get Started
-
It's the simple way to write a new application in Node.js:
  
    var toolkit = require('jsdx-toolkit');
    
    /* Create a new application */
    var app = new toolkit.Application('Clutter Application');
    
    /* Create a new window */
    app.createWindow(function(window) {
    
      /* Quit current application when this window is destroyed */
      window.on(toolkit.EVENT_DESTROY, function() {
        app.quit();
      });

      window.title = 'Application Window';
      window.hasToolbar = true;
      window.show();
    });
    
    app.run();

License
-
Licensed under the MIT License

Authors
-
Copyright(c) 2012 Fred Chien <<fred@mandice.com>>

Copyright
-
Copyright(c) 2012 Mandice Company.
(http://www.mandice.com/)
