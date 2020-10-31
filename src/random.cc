#include <napi.h>

#include "random.hh"

// JavaScript bindings

Napi::Object Random::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Random", {
    InstanceMethod("setSeed", &Random::SetSeed),
    InstanceMethod("next", &Random::Next),
    InstanceMethod("nextBytes", &Random::NextBytes),
    InstanceMethod("nextInt", &Random::NextInt),
    InstanceMethod("nextLong", &Random::NextLong),
    InstanceMethod("nextBoolean", &Random::NextBoolean),
    InstanceMethod("nextFloat", &Random::NextFloat),
    InstanceMethod("nextDouble", &Random::NextDouble),
    InstanceMethod("nextGaussian", &Random::NextGaussian),
  });
  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("Random", func);

  return exports;
}

Random::Random(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Random>(info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length != 1 || !info[0].IsBigInt()) {
    Napi::TypeError::New(env, "BigInt expected for seed").ThrowAsJavaScriptException();
    return;
  }

  bool lossless = true;
  int64_t seed = info[0].As<Napi::BigInt>().Int64Value(&lossless);

  if (!lossless) {
    Napi::TypeError::New(env, "BigInt out of range for lossless conversion").ThrowAsJavaScriptException();
    return;
  }

  _random.setSeed(seed);
}

Napi::Value Random::SetSeed(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length != 1 || !info[0].IsBigInt()) {
    Napi::TypeError::New(env, "BigInt expected for seed").ThrowAsJavaScriptException();
    return env.Null();
  }

  bool lossless = true;
  int64_t seed = info[0].As<Napi::BigInt>().Int64Value(&lossless);

  if (!lossless) {
    Napi::TypeError::New(env, "BigInt out of range for lossless conversion").ThrowAsJavaScriptException();
    return env.Null();
  }

  _random.setSeed(seed);

  return env.Undefined();
}

Napi::Value Random::Next(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length != 1 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected for bit length").ThrowAsJavaScriptException();
    return env.Null();
  }

  int32_t bits = info[0].As<Napi::Number>().Int32Value();

  return Napi::Number::New(env, _random.next(bits));
}

Napi::Value Random::NextBytes(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length != 2) {
    Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsTypedArray()) {
    Napi::TypeError::New(env, "Expected a typed array").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Uint8Array buffer = info[0].As<Napi::Uint8Array>();

  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "Expected a number length").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Number lengthArg = info[1].As<Napi::Number>();

  int32_t byteLength = lengthArg.Int32Value();

  _random.nextBytes(buffer.Data(), byteLength);

  return env.Undefined();
}

Napi::Value Random::NextInt(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length == 0) {
    return Napi::Number::New(env, _random.nextInt());
  } else if (length == 1 && info[0].IsNumber()) {
    int32_t n = info[0].As<Napi::Number>().Int32Value();
    if (n <= 0) {
      Napi::TypeError::New(env, "Expected a positive integer").ThrowAsJavaScriptException();
      return env.Null();
    }
    return Napi::Number::New(env, _random.nextInt(n));
  } else {
    Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Random::NextLong(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length != 0) {
    Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  int64_t val = _random.nextLong();

  return Napi::BigInt::New(env, val);
}


Napi::Value Random::NextBoolean(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length != 0) {
    Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  bool val = _random.nextBoolean();

  return Napi::Boolean::New(env, val);
}

Napi::Value Random::NextFloat(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length != 0) {
    Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  float val = _random.nextFloat();

  return Napi::Number::New(env, val);
}

Napi::Value Random::NextDouble(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length != 0) {
    Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  double val = _random.nextDouble();

  return Napi::Number::New(env, val);
}

Napi::Value Random::NextGaussian(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();

  if (length != 0) {
    Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  double val = _random.nextGaussian();

  return Napi::Number::New(env, val);
}

// Export


Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  Random::Init(env, exports);
  return exports;
}
NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll);
