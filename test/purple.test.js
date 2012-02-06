var vows = require('vows');
var assert = require('assert');
var purple = require('../lib/purple');

vows.describe('instant messages').addBatch({
    'setting invalid user directory': {
        topic: purple.util.setUserDir(),

        'result is undefined': function (result) {
            assert.equal (result, undefined);
        }
    },
    'setting valid user directory': {
        topic: purple.util.setUserDir("/does/not/need/to/exist"),

        'result is undefined': function (result) {
            assert.equal (result, undefined);
        }
    },
    'enable debugging': {
        topic: purple.debug.setEnabled(true),

        'result is undefined': function (result) {
            assert.equal (result, undefined);
        }
    },
    'disable debugging': {
        topic: purple.debug.setEnabled(false),

        'result is undefined': function (result) {
            assert.equal (result, undefined);
        }
    },
    'install no ui callbacks': {
        topic: purple.core.setUiOps(),

        'result is undefined': function (result) {
            assert.equal (result, undefined);
        }
    },
    'install ui callbacks': {
        topic: purple.core.setUiOps({
          "ui_prefs_init" : function() { conole.log("ui_prefs_init called"); },
          "debug_ui_init" : function() { conole.log("debug_ui_init called"); },
          "ui_init" : function() { conole.log("ui_init called"); }
        }),

        'result is undefined': function (result) {
            assert.equal (result, undefined);
        }
    }
}).run();
