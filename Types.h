// Author: Jake Rieger
// Created: 8/21/2024.
//

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <limits>
#include <filesystem>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <ranges>
#include <thread>
#include <unordered_map>

#pragma region Primitive Types
/// Unsigned 8-bit integer
using u8 = uint8_t;
/// Unsigned 16-bit integer
using u16 = uint16_t;
/// Unsigned 32-bit integer
using u32 = uint32_t;
/// Unsigned 64-bit integer
using u64 = uint64_t;
/// Signed 8-bit integer
using i8 = int8_t;
/// Signed 16-bit integer
using i16 = int16_t;
/// Signed 32-bit integer
using i32 = int32_t;
/// Signed 64-bit integer
using i64 = int64_t;
/// 32-bit floating point number
using f32 = float;
/// 64-bit floating point number
using f64 = double;
/// UTF-16 encoded wide string
using wstr = std::wstring;
/// UTF-8 encoded string
using str = std::string;
#pragma endregion

#if defined(CAST_ALIASES) || defined(ALL_ALIASES)
    #define CAST static_cast
    #define RCAST reinterpret_cast
    #define CCAST const_cast
    #define DCAST dynamic_cast
#endif

#if defined(STL_ALIASES) || defined(ALL_ALIASES)
using Exception    = std::exception;
using RuntimeError = std::runtime_error;

using Mutex  = std::mutex;
using Thread = std::thread;

template<typename T>
using Queue = std::queue<T>;

template<typename T>
using Atomic = std::atomic<T>;

template<typename T>
using Function = std::function<T>;

namespace FileSystem = std::filesystem;
using Path           = std::filesystem::path;

template<class T>
using Shared = std::shared_ptr<T>;

template<class T>
using Weak = std::weak_ptr<T>;

template<class T>
using Option = std::optional<T>;

template<class T>
using Unique = std::unique_ptr<T>;

template<class T, class D>
using UniqueDelete = std::unique_ptr<T, D>;

template<class T>
using Vector = std::vector<T>;

template<class K, class V>
using Dictionary = std::unordered_map<K, V>;

template<class K, class V>
using Map = std::map<K, V>;
#endif

#if defined(NULL_ALIASES) || defined(ALL_ALIASES)
static constexpr std::nullopt_t kNone = std::nullopt;
static constexpr auto None            = nullptr;
static constexpr auto Ignore          = std::ignore;
#endif

#if defined(INFINITY_ALIASES) || defined(ALL_ALIASES)
constexpr auto Inf32 = std::numeric_limits<float>::infinity();
constexpr auto Inf64 = std::numeric_limits<double>::infinity();
#endif

#if defined(STL_HELPERS) || defined(ALL_ALIASES)
template<typename E>
constexpr auto EnumRange() {
    static_assert(std::is_enum_v<E>, "EnumRange can only be used with enums.");
    return std::ranges::iota_view(
      E {},
      static_cast<E>(static_cast<std::underlying_type_t<E>>(E::Last) + 1));
}
#endif