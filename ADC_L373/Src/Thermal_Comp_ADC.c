/*
 * Thermal_Comp_ADC.c
 *
 *  Created on: 27 dic 2017
 *      Author: tommy
 */

#include "stm32f3xx_hal.h"
#include <stdio.h>


const int8_t ADC_Calib [] = {
5,5,5,5,5,5,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,
1,1,1,1,1,1,1,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-2,-2,-2,
-2,-2,-2,-2,-2,-3,-3,-3,-3,-3,-3,-3,-4,-4,-4,-4,-4,-4,-4,-5,
-5};

const int8_t ADC_Temp [] = {
-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,
13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,
35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
57,58,59,60,61};


int16_t Temp_Comp (int16_t data, int16_t temp){
	uint8_t i;
	/* check table alignment */
	if (sizeof(ADC_Temp) != sizeof (ADC_Calib)){
		return data;
	}

	/* find actual temp */
	if (temp <= ADC_Temp[0]){
		data = data - (int16_t)ADC_Calib [0];
	}
	else if (temp >= ADC_Temp[sizeof(ADC_Temp) - 1]){
		data = data - (int16_t)ADC_Calib [sizeof(ADC_Temp) - 1];
	}
	else {
		for (i=0;i<sizeof(ADC_Temp);i++){
			if (ADC_Temp [i] == temp){
				data = data - (int16_t)ADC_Calib [i];
			}
		}
	}

	return data;
}
