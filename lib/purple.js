var native = require('../build/Release/purple');
var debug = module.exports.debug = {};
var util = module.exports.util = {};
var core = module.exports.core = {};

/**
 * Set the user dir.
 *
 * @param dir the directory to set.
 */
util.setUserDir = function(dir) {
  if (dir) {
    return native.purple_util_set_user_dir(dir);
  }
};

/**
 * Enable or disable debugging.
 *
 * @param flag indicate whether or not debug should be enabled
 */
debug.setEnabled = function(flag) {
  return native.purple_debug_set_enabled(flag === true);
};

/**
 * Installs the callback for UI.
 *
 * @param options the callbacks by name as specified in {@link http://developer.pidgin.im/doxygen/dev/html/structPurpleCoreUiOps.html}
 * @see "http://developer.pidgin.im/doxygen/dev/html/core_8h.html#b11f672706d2023bcd8f0030f54e9288"
 */
core.setUiOps = function(options) {
  options = options || {};

  return native.purple_core_set_ui_ops(options.ui_prefs_init || function() {},
     options.debug_ui_init || function() {}, 
     options.ui_init || function() {});
};