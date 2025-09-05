#pragma once
#include <array>
#include <atomic>
#include <cstddef>
#include <type_traits>

using std::array;
using std::atomic;
using std::size_t;
using std::memory_order_relaxed;
using std::memory_order_acquire;
using std::memory_order_release;
using std::is_trivially_copyable_v;

template <typename T, size_t N>
class SpscRing {
  static_assert((N & (N - 1)) == 0, "N must be power of two");
  static_assert(is_trivially_copyable_v<T>, "T must be trivially copyable");

  alignas(64) array<T, N> buf_;
  alignas(64) atomic<size_t> head_{0};
  alignas(64) atomic<size_t> tail_{0};

public:
  bool push(const T& v) noexcept {
    auto t = tail_.load(memory_order_relaxed);
    auto h = head_.load(memory_order_acquire);
    if (((t + 1) & (N - 1)) == (h & (N - 1))) return false;
    buf_[t & (N - 1)] = v;
    tail_.store(t + 1, memory_order_release);
    return true;
  }

  bool pop(T& out) noexcept {
    auto h = head_.load(memory_order_relaxed);
    auto t = tail_.load(memory_order_acquire);
    if ((h & (N - 1)) == (t & (N - 1))) return false;
    out = buf_[h & (N - 1)];
    head_.store(h + 1, memory_order_release);
    return true;
  }

  bool empty() const noexcept {
    return head_.load(memory_order_acquire) ==
           tail_.load(memory_order_acquire);
  }
};