#ifndef EEPROM_HPP
#define EEPROM_HPP

#include "i2c_device.hpp"
#include "spi_device.hpp"
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace EEPROM {

    struct EEPROM {
        template <typename T>
        void write_memory(std::uint8_t const reg_address, T const& value) const noexcept
        {
            if (!this->serial_device.valueless_by_exception()) {
                std::visit(
                    [reg_address, &value](auto const& device) {
                        device.template write_bytes(reg_address, Utility::value_to_bytes(value));
                    },
                    this->serial_device);
            }
        }

        template <typename T>
        T read_memory(std::uint8_t const reg_address) const noexcept
        {
            if (!this->serial_device.valueless_by_exception()) {
                return std::visit(
                    [reg_address](auto const& device) {
                        return Utility::bytes_to_value<T>(device.template read_bytes<sizeof(T)>(reg_address));
                    },
                    this->serial_device);
            }
            return T{};
        }

        template <typename T>
        void write_memory(std::string_view const& name, T const& value) noexcept
        {
            if (!this->serial_device.valueless_by_exception() &&
                this->memory_pointer + sizeof(T) <= this->memory_size) {
                std::visit(
                    [&](auto const& device) {
                        device.template write_bytes(this->memory_pointer, Utility::value_to_bytes(value));
                    },
                    this->serial_device);

                this->memory_map.try_emplace(this->memory_map.end(), name, this->memory_pointer, sizeof(T));
                this->memory_pointer += sizeof(T);
            }
        }

        template <typename T>
        T read_memory(std::string_view const& name) noexcept
        {
            if (!this->serial_device.valueless_by_exception() &&
                this->memory_pointer + sizeof(T) <= this->memory_size) {
                auto memory{this->memory_map.at(name)};

                if (sizeof(T) == memory.bytes) {
                    return std::visit(
                        [&](auto const& device) {
                            return Utility::bytes_to_value<T>(device.template read_bytes<sizeof(T)>(memory.address));
                        },
                        this->serial_device);
                }
            }
            return T{};
        }

        std::variant<Utility::I2CDevice, Utility::SPIDevice> serial_device{};

        std::uint8_t memory_size{};
        std::uint8_t memory_pointer{0U};

        struct Memory {
            std::uint8_t address{};
            std::size_t bytes{};
        };

        std::unordered_map<std::string_view, Memory> memory_map{};
    };

} // namespace EEPROM

#endif // EEPROM_HPP