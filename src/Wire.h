#pragma once
#include <cstdint>
#include <type_traits>

enum class Cmd  : uint8_t { New, Cancel };
enum class Side : uint8_t { Buy, Sell };
enum class OrdType : uint8_t { Limit, Market };

using SymbolId = uint16_t;
using OrderId  = uint64_t;
using Price    = uint64_t;
using Qty      = int64_t;
using TsNanos  = uint64_t;

struct OrderCmd {
  Cmd      cmd;
  Side     side;
  OrdType  type;
  SymbolId sym;
  OrderId  id;
  Price    px;
  Qty      qty;
  TsNanos  ts_ns;
};
static_assert(std::is_trivially_copyable<OrderCmd>::value, "OrderCmd must be trivially copyable");
