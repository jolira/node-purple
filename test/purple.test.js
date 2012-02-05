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
    }
}).run();
