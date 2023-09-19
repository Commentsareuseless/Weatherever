/**
 * @file CortexM0.hpp
 * @author Commentsareuseless
 * @brief Peripheral setup and driver interface
 * @date 2023-09-11
 * 
 * Copyright: Macieg :)
 * 
 */
#pragma once

#include "PUsart.hpp"
#include "PSpi.hpp"
#include "PI2C.hpp"

namespace drv
{

class CortexM0Plus
{
public:
  static void InitHalSystem();
  static void InitPeripherals();

  Usart USART1;
  Usart UASRT2;

  Spi SPI1;

  I2C I2C1;
  I2C I2C2;
};

} // namespace drv