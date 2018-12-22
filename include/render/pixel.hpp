#pragma once

namespace donkey {
namespace render {
namespace pixel {

enum class InternalFormat
{
  kRGB8,
  kRGBA8,
  kDepthComponent24
};

enum class Format
{
  kRGB,
  kRGBA,
  kDepthComponent,
};

enum class ComponentType
{
  kUnsignedByte,
  kFloat
};

}
}
}
