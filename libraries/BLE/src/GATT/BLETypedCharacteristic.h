/*
  Copyright (c) 2016 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
*/

#ifndef __BLE_TYPED_CHARACTERISTIC_H
#define __BLE_TYPED_CHARACTERISTIC_H

#include <stdint.h>
#include "BLECharacteristic.h"

template<typename T>
class BLETypedCharacteristic : public BLECharacteristic {
	public:
        BLETypedCharacteristic(const char *uuid, uint8_t properties);
		BLETypedCharacteristic(const char *uuid, uint8_t properties, T initData);
        
        void setValue(T value);
        T &getValue(void);
        
	private:
        T _value;
};
		
#endif