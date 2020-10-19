const { Random } = require("bindings")("javaRandom");

module.exports = Random;
Object.defineProperty(module.exports, "default", {
  value: Random,
});
