/*
 * System.hpp
 *
 *  Created on: Mar 4, 2024
 *      Author: user
 */

#ifndef INC_APP_SYSTEM_HPP_
#define INC_APP_SYSTEM_HPP_

#include <stm32f4xx.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>


class System{
public:

	static void errorHandler(){
		while(1){

		}
	}

	static uint32_t getTick(){
		return HAL_GetTick();
	}


	static int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max, bool doNotOverlap=false){
		int32_t val = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		if(doNotOverlap){
			if(val > out_max){
				return out_max;
			}else if(val < out_min){
				return out_max;
			}
		}
		return val;
	}



};







#endif /* INC_APP_SYSTEM_HPP_ */
