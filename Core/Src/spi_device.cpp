#include "spi_device.hpp"
#include "stm32l4xx_hal.h"

namespace Utility {

    SPIDevice::SPIDevice(SPIHandle const spi_bus, GPIOHandle const gpio, std::uint16_t const chip_select) noexcept :
        chip_select_{chip_select}, spi_bus_{spi_bus}, gpio_{gpio}
    {
        this->initialize();
    }

    void SPIDevice::transmit_dword(std::uint32_t const dword) const noexcept
    {
        this->transmit_dwords(std::array<std::uint32_t, 1UL>{dword});
    }

    void SPIDevice::transmit_word(std::uint16_t const word) const noexcept
    {
        this->transmit_words(std::array<std::uint16_t, 1UL>{word});
    }

    void SPIDevice::transmit_byte(std::uint8_t const byte) const noexcept
    {
        this->transmit_bytes(std::array<std::uint8_t, 1UL>{byte});
    }

    std::uint32_t SPIDevice::receive_dword() const noexcept
    {
        return this->receive_dwords<1UL>()[0];
    }

    std::uint16_t SPIDevice::receive_word() const noexcept
    {
        return this->receive_words<1UL>()[0];
    }

    std::uint8_t SPIDevice::receive_byte() const noexcept
    {
        return this->receive_bytes<1UL>()[0];
    }

    std::uint32_t SPIDevice::read_dword(std::uint8_t const reg_address) const noexcept
    {
        return this->read_dwords<1UL>(reg_address)[0];
    }

    std::uint16_t SPIDevice::read_word(std::uint8_t const reg_address) const noexcept
    {
        return this->read_words<1UL>(reg_address)[0];
    }

    std::uint8_t SPIDevice::read_byte(std::uint8_t const reg_address) const noexcept
    {
        return this->read_bytes<1UL>(reg_address)[0];
    }

    std::uint8_t SPIDevice::read_bits(std::uint8_t const reg_address,
                                      std::uint8_t const position,
                                      std::size_t const size) const noexcept
    {
        return get_bits(this->read_byte(reg_address), size, position);
    }

    bool SPIDevice::read_bit(std::uint8_t const reg_address, std::uint8_t const position) const noexcept
    {
        return get_bit(this->read_byte(reg_address), position);
    }

    void SPIDevice::write_dword(std::uint8_t const reg_address, std::uint32_t const dword) const noexcept
    {
        this->write_dwords(reg_address, std::array<std::uint32_t, 1UL>{dword});
    }

    void SPIDevice::write_word(std::uint8_t const reg_address, std::uint16_t const word) const noexcept
    {
        this->write_words(reg_address, std::array<std::uint16_t, 1UL>{word});
    }

    void SPIDevice::write_byte(std::uint8_t const reg_address, std::uint8_t const byte) const noexcept
    {
        this->write_bytes(reg_address, std::array<std::uint8_t, 1UL>{byte});
    }

    void SPIDevice::write_bits(std::uint8_t const reg_address,
                               std::uint8_t const bits,
                               std::uint8_t const position,
                               std::size_t const size) const noexcept
    {
        std::uint8_t write{this->read_byte(reg_address)};
        set_bits(write, bits, size, position);
        this->write_byte(reg_address, write);
    }

    void
    SPIDevice::write_bit(std::uint8_t const reg_address, bool const bit, std::uint8_t const position) const noexcept
    {
        std::uint8_t write{this->read_byte(reg_address)};
        set_bit(write, bit, position);
        this->write_byte(reg_address, write);
    }

    void SPIDevice::initialize() noexcept
    {
        if (this->spi_bus_ != nullptr && this->gpio_ != nullptr) {
            HAL_GPIO_WritePin(this->gpio_, this->chip_select_, GPIO_PinState::GPIO_PIN_SET);
            this->initialized_ = true;
        }
    }

    void SPIDevice::deinitialize() noexcept
    {
        if (this->spi_bus_ != nullptr && this->gpio_ != nullptr) {
            HAL_GPIO_WritePin(this->gpio_, this->chip_select_, GPIO_PinState::GPIO_PIN_RESET);
            this->initialized_ = false;
        }
    }

}; // namespace Utility