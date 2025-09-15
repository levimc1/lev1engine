#pragma once

#include <engine/core/taskm/task.hpp>
#include <cstdint>
#include <cstring>

namespace taskm {

  namespace internal {
    const unsigned int MAX_FLAG_BYTES = 64;
    const unsigned int MAX_ADDRESS_SIZE = 16;
    struct Flag {
      std::aligned_storage_t<MAX_FLAG_BYTES> data;

      template<typename T>
      static Flag make_default() {
        Flag f;
        static_assert(sizeof(T) <= MAX_FLAG_BYTES, "A típus túl nagy, növeld a MAX_FLAG_BYTES-ot!");
        static_assert(std::is_trivially_copyable_v<T>, "Nem triviálisan másolható (ötletem sincs mit!");
        new (&f.data) T{};
        return f;
      }

      template<typename T>
      T& get() {
        static_assert(sizeof(T) <= MAX_FLAG_BYTES, "A típus túl nagy, növeld a MAXname_FLAG_BYTES-ot!");
        static_assert(std::is_trivially_copyable_v<T>, "A típusod nem triviálisan másolható!");
        return *reinterpret_cast<T*>(&data);
      }
    };

    struct Address {
      std::array<char, MAX_ADDRESS_SIZE> data;
      uint8_t size;
    
      template<typename T> // type
      Address(std::initializer_list<T> values) : size(static_cast<uint8_t>(values.size())) {
        static_assert(std::is_trivially_copyable_v<T>, "T-nek 'trivially copyable'-nak kell lennie! a jelenlegi T-d ezek szerint nem az.");
        if (values.size() * sizeof(T) > MAX_ADDRESS_SIZE) {
          throw std::invalid_argument("Address túl nagy! Állítsd át a ADDRESS_SIZE-t!");
        }
        memcpy(data.data(), values.begin(), values.size() * sizeof(T));
      }
    
      inline bool operator==(const Address& other) const noexcept {
        if (size != other.size) return false;
        return std::memcmp(data.data(), other.data.data(), size) == 0;
      }

      struct Hash {
        std::size_t operator()(const Address& ad) const noexcept {
          return std::hash<std::string_view>()(std::string_view(ad.data.data(), ad.size));
       }
      };
    };

    inline std::unordered_map<internal::Address, Flag, internal::Address::Hash> flags;
  }


  template<typename F,typename T>
  F& flag(std::initializer_list<T> values) {
    internal::Address address = internal::Address(values);
    auto it = internal::flags.find(address);
    if (it == internal::flags.end()) {
      it = internal::flags.insert({address, internal::Flag::make_default<F>()}).first; // Alap, nem tudom mi lehet ez... 
    }
    return it->second.get<F>();
  } // szimpla flag módosítás :)

}