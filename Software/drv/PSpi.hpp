/**
 * @file PSpi.hpp
 * @author Commentsareuseless
 * @brief SPI peripheral driver
 * @date 2023-09-12
 *
 * Copyright: Macieg :)
 *
 */
#pragma once

#include "CommHandler.hpp"

namespace drv
{
constexpr auto SPI_MAX_RECV_BUFF_SIZE{128u};
constexpr auto SPI_MAX_SEND_BUFF_SIZE{128u};

class Spi : public CommHandler<SPI_MAX_RECV_BUFF_SIZE, SPI_MAX_SEND_BUFF_SIZE>
{
public:
  void Init();
  bool TriggerSend();
  bool TriggerRecv();
  CommStatus GetRecvStatus();
  CommStatus GetSendStatus();
};
} // namespace drv
