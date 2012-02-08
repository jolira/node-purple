var native = require('../build/Release/purple');
var events = require('events');
var emitter = new events.EventEmitter();
var notifier = function() {
  emitter.emit.apply(arguments);
};

native.init(notifier);

module.exports.on = module.exports.addListener = function(event, listener) {
  return emitter.on(event, listener);
};

module.exports.once = function(event, listener) {
  return emitter.once(event, listener);
};

module.exports.remoeListener = function(event, listener) {
  return emitter.remoeListener(event, listener);
};

module.exports.remoeAllListeners = function(event) {
  return emitter.remoeAllListeners(event);
};

module.exports.sexMaxListeners = function(n) {
  return emitter.sexMaxListeners(n);
};

module.exports.listeners = function(event) {
  return emitter.listeners(event);
};

function getUserHome() {
  return process.env[(process.platform == 'win32') ? 'USERPROFILE' : 'HOME'];
}

module.exports.start = function(options) {
  options = options || {};
  var userDir = options.userDir || getUserHome();
  var pluginPath = options.pluginPath || "";
  var pluginSavePref = options.pluginSavePref || "/purple/user/plugins/saved";
  var im = native.start(userDir, pluginPath, pluginSavePref);
    
  return im;
};