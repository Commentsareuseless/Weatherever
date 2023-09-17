/**
 * @file CommHandler.hpp
 * @author Commentsareuseless
 * @brief Base class for all communication interfaces
 * @date 2023-09-15
 *
 * Copyright: Macieg :)
 *
 */
#pragma once

#include <cstdint>

#include <etl/algorithm.h>
#include <etl/array.h>
#include <etl/circular_buffer.h>

namespace drv
{

template <uint32_t RxBuffSize, uint32_t TxBuffSize>
class CommHandler
{
public:
  enum CommStatus : uint8_t
  {
    OK,      /* Transmission/Reception OK */
    PENDING, /* Transmission/Reception in progress */
    FAILED   /* Error during transmission/reception */
  };

  bool ReadByte(uint8_t& recvByte) {
    bool retval{false};

    if (!recvBuff.empty()) {
      recvByte = recvBuff.front();
      recvBuff.pop();
      retval = true;
    }

    return retval;
  }

  bool ReadDataBatch(uint8_t* data, uint32_t dataSize) {
    bool retval{false};
    const auto maxRead{etl::min(recvBuff.size(), dataSize)};

    if ((!recvBuff.empty()) && (nullptr != data)) {
      for (uint32_t idx{0}; idx < maxRead; ++idx) {
        data[idx] = recvBuff.front();
        recvBuff.pop();
      }
      retval = true;
    }

    return retval;
  }

  template <uint32_t dataSize>
  bool ReadDataBatch(etl::array<uint8_t, dataSize>& data) {
    bool retval{false};
    constexpr auto maxRead{etl::min(recvBuff.size(), dataSize)};

    if (!recvBuff.empty()) {
      for (uint32_t idx{0}; idx < maxRead; ++idx) {
        data[idx] = recvBuff.front();
        recvBuff.pop();
      }
      retval = true;
    }

    return retval;
  }

  bool SendByte(const uint8_t byteToSend) {
    // We don't care if the oldest val will be overriten
    // so, no additional checks are required;
    sendBuff.push(byteToSend);

    return true;
  }

  bool SendDataBatch(const uint8_t* const data, uint32_t dataSize) {
    bool retval{false};
    // Trying to send more than buffer cappacity makes no sense
    const uint32_t maxWrite{etl::min(dataSize, sendBuff.max_size())};

    for (uint32_t idx{0}; idx < maxWrite; ++idx) { sendBuff.push(data[idx]); }

    return retval;
  }

  template <uint32_t dataSize>
  bool SendDataBatch(const etl::array<uint8_t, dataSize>& data) {
    bool retval{false};
    // Trying to send more than buffer cappacity makes no sense
    constexpr uint32_t maxWrite{etl::min(data.size(), sendBuff.max_size())};

    for (uint32_t idx{0}; idx < maxWrite; ++idx) {
      sendBuff.push(data.at(idx));
    }

    return retval;
  }

  constexpr decltype(RxBuffSize) rxSize() { return recvBuff.size(); }
  constexpr decltype(RxBuffSize) rxCappacity() { return recvBuff.capacity(); }
  constexpr bool canReceiveData() { return !recvBuff.full(); }
  constexpr decltype(TxBuffSize) txSize() { return sendBuff.size(); }
  constexpr decltype(TxBuffSize) txCappacity() { return sendBuff.capacity(); }
  constexpr bool hasDataToSend() { return !sendBuff.empty(); }

private:
  /**
     * @note This handle is void* bcs we want to
     *       eliminate all dependencies on generated drivers
     */
  void* ifHandle{nullptr};

  /**
   * @brief All messages are written here from interrupt.
   *        Read*() functions read from here, actual transmission
   *        is handled by DMA
   */
  etl::circular_buffer<uint8_t, RxBuffSize> recvBuff;

  /**
   * @brief All messages to transmit are written here
   *        Send*() functions write here, actual transmission
   *        is handled by DMA
   */
  etl::circular_buffer<uint8_t, TxBuffSize> sendBuff;
};
} // namespace drv