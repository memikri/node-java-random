#pragma once

#include <napi.h>

#include "java_random.hh"

// JavaScript bindings

class Random : public Napi::ObjectWrap<Random>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Random(const Napi::CallbackInfo &info);

private:
  Napi::Value SetSeed(const Napi::CallbackInfo &info);
  Napi::Value Next(const Napi::CallbackInfo &info);
  Napi::Value NextBytes(const Napi::CallbackInfo &info);
  Napi::Value NextInt(const Napi::CallbackInfo &info);
  Napi::Value NextLong(const Napi::CallbackInfo &info);
  Napi::Value NextBoolean(const Napi::CallbackInfo &info);
  Napi::Value NextFloat(const Napi::CallbackInfo &info);
  Napi::Value NextDouble(const Napi::CallbackInfo &info);
  Napi::Value NextGaussian(const Napi::CallbackInfo &info);

  JavaRandom _random;
};
