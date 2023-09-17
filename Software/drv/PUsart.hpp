/**
 * @file PUsart.hpp
 * @author Commentsareuseless
 * @brief USART peripheral driver
 * @date 2023-09-12
 * 
 * Copyright: Macieg :)
 * 
 */
#pragma once

#include "CommHandler.hpp"

namespace drv
{
constexpr auto USART_MAX_RECV_BUFF_SIZE{256u};
constexpr auto USART_MAX_SEND_BUFF_SIZE{256u};

class Usart :
    public CommHandler<USART_MAX_RECV_BUFF_SIZE, USART_MAX_SEND_BUFF_SIZE>
{
public:
  void Init();
  bool TriggerSend();
  bool TriggerRecv();
  CommStatus GetRecvStatus();
  CommStatus GetSendStatus();
};
} // namespace drv
