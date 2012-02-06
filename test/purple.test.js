var vows = require('vows');
var assert = require('assert');
var purple = require('../lib/purple');

purple.on("ui_prefs_init", function() {
  console.log("ui_prefs_init", arguments);
});

purple.on("debug_init", function() {
  console.log("debug_init", arguments);
});

purple.on("ui_init", function() {
  console.log("ui_init", arguments);
});

vows.describe('instant messages').addBatch({
    'connect': {
        topic: purple.start(),

        'result is undefined': function (result) {
            assert.equal (result, undefined);
        }
    }
}).run();
