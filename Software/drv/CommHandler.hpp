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

#include <cstddef>
#include <cstdint>

#include <etl/algorithm.h>
#include <etl/array.h>
#include <etl/circular_buffer.h>
#include <stdint.h>

namespace drv
{
using SizeType = size_t;

template <SizeType RxBuffSize, SizeType TxBuffSize>
class CommHandler
{
public:
  enum CommStatus : uint8_t
  {
    OK,      /* Transmission/Reception OK */
    PENDING, /* Transmission/Reception in progress */
    FAILED   /* Error during transmission/reception */
  };

  bool ReadByte(uint8_t& recvByte) { return (popFromRx(&recvByte, 1) == 1); }

  SizeType ReadDataBatch(uint8_t* data, SizeType dataSize) {
    return popFromRx(data, dataSize);
  }

  template <SizeType dataSize>
  SizeType ReadDataBatch(etl::array<uint8_t, dataSize>& data) {
    return popFromRx(data.data(), data.size());
  }

  bool SendByte(const uint8_t byteToSend) {
    return (pushToTx(&byteToSend, 1) == 1);
  }

  SizeType SendDataBatch(const uint8_t* const data, SizeType dataSize) {
    return pushToTx(data, dataSize);
  }

  template <SizeType dataSize>
  SizeType SendDataBatch(const etl::array<uint8_t, dataSize>& data) {
    return pushToTx(data.data(), data.size());
  }

  constexpr decltype(RxBuffSize) rxSize() { return recvBuff.size(); }
  constexpr decltype(RxBuffSize) rxCappacity() { return recvBuff.capacity(); }
  constexpr bool canReceiveData() { return !recvBuff.full(); }
  constexpr decltype(TxBuffSize) txSize() { return sendBuff.size(); }
  constexpr decltype(TxBuffSize) txCappacity() { return sendBuff.capacity(); }
  constexpr bool hasDataToSend() { return !sendBuff.empty(); }
  constexpr void flushRx() { clearBuff(recvBuff); }
  constexpr void flushTx() { clearBuff(sendBuff); }

protected:
  SizeType pushToRx(const uint8_t* dataBuff, SizeType dataSize) {
    return pushToBuff(recvBuff, dataBuff, dataSize);
  }

  SizeType popFromRx(uint8_t* dataBuff, SizeType dataSize) {
    return popFromBuff(recvBuff, dataBuff, dataSize);
  }

  SizeType pushToTx(const uint8_t* dataBuff, SizeType dataSize) {
    return pushToBuff(sendBuff, dataBuff, dataSize);
  }

  SizeType popFromTx(uint8_t* dataBuff, SizeType dataSize) {
    return popFromBuff(sendBuff, dataBuff, dataSize);
  }

  void* getInternalHandle() { return ifHandle; }

private:
  template <typename DataType, SizeType buffSize>
  using GenericBuff = etl::circular_buffer<DataType, buffSize>;
  using RxBuff_t    = GenericBuff<uint8_t, RxBuffSize>;
  using TxBuff_t    = GenericBuff<uint8_t, TxBuffSize>;

  template <typename DataType, SizeType buffSize>
  constexpr static SizeType popFromBuff(GenericBuff<DataType, buffSize>& buff,
                                        uint8_t* dataBuff,
                                        SizeType dataSize) {
    SizeType retval{0u};
    const auto maxRead{etl::min(buff.size(), dataSize)};

    for (SizeType idx{0}; idx < maxRead; ++idx) {
      dataBuff[idx] = buff.front();
      buff.pop();
      ++retval;
    }

    return retval;
  }

  template <typename DataType, SizeType buffSize>
  constexpr static SizeType pushToBuff(GenericBuff<DataType, buffSize>& buff,
                                       uint8_t* dataBuff,
                                       SizeType dataSize) {
    SizeType retval{0u};
    // Allow overriding data but
    // trying to write more than buffer cappacity makes no sense
    const SizeType maxWrite{etl::min(dataSize, buff.max_size())};

    for (SizeType idx{0}; idx < maxWrite; ++idx) {
      buff.push(dataBuff[idx]);
      ++retval;
    }

    return retval;
  }

  template <typename DataType, SizeType buffSize>
  constexpr static void clearBuff(GenericBuff<DataType, buffSize>& buff) {
    buff.clear();
    buff.fill(0x00);
  }

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
  RxBuff_t recvBuff;

  /**
   * @brief All messages to transmit are written here
   *        Send*() functions write here, actual transmission
   *        is handled by DMA
   */
  TxBuff_t sendBuff;
};
} // namespace drv