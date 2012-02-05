var native = require('../build/Release/purple');
var debug = module.exports.debug = {};
var util = module.exports.util = {};

/**
 * Set the user dir.
 *
 * @param dir the directory to set.
 */
util.setUserDir = function(dir) {
  if (dir) {
    native.purple_util_set_user_dir(dir);
  }
};

/**
 * Enable or disable debugging.
 *
 * @param flag indicate whether or not debug should be enabled
 */
debug.setEnabled = function(flag) {
  native.purple_debug_set_enabled(flag === true);
};