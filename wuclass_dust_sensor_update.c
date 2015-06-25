#include "debug.h"
#include "../../common/native_wuclasses/native_wuclasses.h"
#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define MIN_RATE	30	// 1.5S
#define MAX_RATE     300	// 15S

//Global variable for calculation of output
int AIN=1;
int cout1=0;
int cout2=0;
int sum1=0;
int sum2=0;
int total1=0;
int total2=0;
float r1=0.0;
float r2=0.0;
int16_t push;

void wuclass_dust_sensor_setup(wuobject_t *wuobject) {}

void wuclass_dust_sensor_update(wuobject_t *wuobject) {
	//local variable

	uint8_t channel=AIN;
	int16_t datarate;
	int16_t this_output;
	int16_t last_output;

	//jump between analogy input from channel 1 to channel 2
	switch(AIN) {
	    case 1:
	    	channel = AIN;
	    	AIN=2;

	    break;

	    case 2:
	    	 channel =AIN;
	    	 AIN=1;

	    break;

	    default:
	    break;
	}

	DEBUG_LOG(DBG_WKPFUPDATE, "channel %d\n",channel);//debug information

	ADCSRA = _BV(ADEN) | (6 & 7);  // set prescaler value
	// Adc.setReference(Adc.INTERNAL);
	ADMUX = (3 << 6) & 0xc0;              // set reference value
	//DEBUG_LOG(DBG_WKPFUPDATE, "ADMUX1 %d\n",ADMUX);
	// light_sensor_reading = Adc.getByte(Adc.CHANNEL0);
	// ADLAR = 1

	//uint8_t channel = 1
	// NOTE: Adc.CHANNEL0 means a value of 0 for the channel variable, but other ADC channels don't map 1-1. For instance channel 15 is selected by setting the channel variable to 39. See below for a list.
	ADMUX = (ADMUX & 0xc0) | _BV(ADLAR) | (channel & 0x0f);
	//DEBUG_LOG(DBG_WKPFUPDATE, "ADMUX2 %d\n",ADMUX);

	ADCSRB |= (channel & 0x20)>>2;

	// do conversion
	ADCSRA |= _BV(ADSC);                  // Start conversion
	while(!(ADCSRA & _BV(ADIF)));         // wait for conversion complete
	ADCSRA |= _BV(ADIF);                  // clear ADCIF
	// After this:
	// 0 <= ADCH <= 255
	// We want to recalibrate this to the range between the low and high values for the DUSTSENSOR

	// to make sure there is no fault output data we read back the last output to compare with this output
	wkpf_internal_read_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_LED_POWER, &last_output);
	//to make sure input value within controlled range to avoid overflow
	wkpf_internal_read_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_DATA_RATE, &datarate);
	if (datarate < MIN_RATE)
		datarate = MIN_RATE*10 ;
		else if (datarate > MAX_RATE)
			datarate = MAX_RATE*10;
		else
			datarate = datarate*10;

	//Get this output value from register
	this_output = (int16_t)(int32_t)ADCH;
	// to make sure there is no fault output data we read back the last output to compare with this output
	 if(abs(this_output - last_output) <= 5)
			   {
		 	 	 this_output=last_output;
			     wkpf_internal_write_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_LED_POWER, last_output);
			     DEBUG_LOG(DBG_WKPFUPDATE, "last_output %d\n",last_output);//debug information
			   }else{
				 this_output=this_output*1;
			     wkpf_internal_write_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_LED_POWER, this_output);
			     DEBUG_LOG(DBG_WKPFUPDATE, "this_output %d\n", this_output);//debug information
			   }
	 //for LED control, two pin for light pollution and heavy pollution
	 int8_t LEDL;
	 int8_t LEDH;
	 DEBUG_LOG(DBG_WKPFUPDATE, "****************push %d****************\n",push);//debug information
	 if (push<100){//this can also be change to wanted value now its 0-100/100-200/over 200
		 LEDL=0;
		 LEDH=0;
		 wkpf_internal_write_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_LED_LOW_PIN, LEDL);
		 wkpf_internal_write_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_LED_HIGH_PIN, LEDH);
		 //DEBUG_LOG(DBG_WKPFUPDATE, "contorlL %d\n", LEDL);
	 }
	 else if(push>=100 && push<=200) {
		 LEDL=100;
		 LEDH=0;
		 wkpf_internal_write_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_LED_LOW_PIN, LEDL);
		 wkpf_internal_write_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_LED_HIGH_PIN, LEDH);
		 //DEBUG_LOG(DBG_WKPFUPDATE, "contorlL %d\n", LEDL);
	 }
	 else{
		 LEDL=100;
		 LEDH=100;
		 wkpf_internal_write_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_LED_LOW_PIN, LEDL);
		 wkpf_internal_write_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_LED_HIGH_PIN, LEDH);
		 //DEBUG_LOG(DBG_WKPFUPDATE, "contorlH %d\n", LEDL);
	 }



	 //count total number of point collected by each channel. And also count the number of low collected for ratio calculation

	 switch(channel) {
	 	    case 1:
	 	    	if (this_output>128 ){
	 	    			cout1=0;
	 	    		}
	 	    		else if (this_output<128){
	 	    			cout1=1;
	 	    		}
	 	    	sum1+=cout1;
	 	    	total1++;
	 	    	DEBUG_LOG(DBG_WKPFUPDATE, "sum1 %d\n",sum1);//debug information
	 	    	DEBUG_LOG(DBG_WKPFUPDATE, "total1 %d\n",total1);//debug information

	 	    break;

	 	    case 2:
	 	    	if (this_output>128 ){
	 	    			cout2=0;
	 	    		}
	 	    		else if (this_output<128){
	 	    			cout2=1;
	 	    		}
	 	    	sum2+=cout2;
	 	    	total2++;
	 	    	DEBUG_LOG(DBG_WKPFUPDATE, "sum2 %d\n",sum2);//debug information
	 	    	DEBUG_LOG(DBG_WKPFUPDATE, "total2 %d\n",total2);//debug information

	 	    break;

	 	    default:
	 	    break;
	 	}

	 //calculate pm2.5 and aqi value, control the output rate
	 if (total1+total2==datarate){
		 r1=100.0*sum1/total1;
		 r2=100.0*sum2/total2;
		 //there are some different formulas that might work, from google groups
		 float weight1 = 0.1776*pow(r1,3) - 0.24*pow(r1,2) + 94.003*r1;
		 float weight2 = 0.1776*pow(r2,3) - 0.24*pow(r2,2) + 94.003*r2;
		 //make sure there is no value below zero
		 if (weight1  < 0.0 ){
			 weight1=0.0;
		 }
		 if (weight2  < 0.0 ){
			 weight2=0.0;
		 }
		 //the pm2.5 is the weight over 1 mines the weight over 2.5
		 float pm2=weight2-weight1;

		 int32_t aqi = 0;
		 	 //to calculate the AQI, we have to use following algorithm to control the accuracy and stability
		 float P25Weight = pm2/15.0;//this number can be changed to a reasonable (1-20) value to fit the condition
		 if (P25Weight>= 0 && P25Weight <= 35) {
		     aqi = 0   + (50.0 / 35 * P25Weight);
		 }
	     else if (P25Weight > 35 && P25Weight <= 75) {
			 aqi = 50  + (50.0 / 40 * (P25Weight - 35));
	     }
	 	 else if (P25Weight > 75 && P25Weight <= 115) {
   	 	     aqi = 100 + (50.0 / 40 * (P25Weight - 75));
		 }
		 else if (P25Weight > 115 && P25Weight <= 150) {
		     aqi = 150 + (50.0 / 35 * (P25Weight - 115));
		 }
		 else if (P25Weight > 150 && P25Weight <= 250) {
		     aqi = 200 + (100.0 / 100.0 * (P25Weight - 150));
		 }
		 else if (P25Weight > 250 && P25Weight <= 500) {
		     aqi = 300 + (200.0 / 250.0 * (P25Weight - 250));
		 }
		 else if (P25Weight > 500.0) {
		     aqi = 500 + (500.0 / 500.0 * (P25Weight - 500.0)); // Extension
		 }
		 else {
		     aqi = 0; // Initializing
		 }

		 push= aqi;
		 //push out the output value
		 wkpf_internal_write_property_int16(wuobject, WKPF_PROPERTY_DUST_SENSOR_OUTPUT, push);

		 //debug information
		 DEBUG_LOG(DBG_WKPFUPDATE, "****************Result1 %d****************\n",(int)r1);
		 DEBUG_LOG(DBG_WKPFUPDATE, "****************Result2 %d****************\n",(int)r2);
		 DEBUG_LOG(DBG_WKPFUPDATE, "****************weight1 %d****************\n",(int)weight1);
		 DEBUG_LOG(DBG_WKPFUPDATE, "****************weight2 %d****************\n",(int)weight2);
		 DEBUG_LOG(DBG_WKPFUPDATE, "****************pm2.5 %d****************\n",(int)pm2);
		 DEBUG_LOG(DBG_WKPFUPDATE, "****************aqi %d****************\n",aqi);

		 //reset all value to zero
		 sum1=0;
		 sum2=0;
		 total1=0;
		 total2=0;
		 r1=0.0;
		 r2=0.0;
		 weight1=0.0;
		 weight2=0.0;
		 pm2=0.0;

	 }
}

// ADC CHANNEL 0,  value for channel variable: 0
// ADC CHANNEL 1,  value for channel variable: 1
// ADC CHANNEL 2,  value for channel variable: 2
// ADC CHANNEL 3,  value for channel variable: 3
// ADC CHANNEL 4,  value for channel variable: 4
// ADC CHANNEL 5,  value for channel variable: 5
// ADC CHANNEL 6,  value for channel variable: 6
// ADC CHANNEL 7,  value for channel variable: 7
// ADC CHANNEL 8,  value for channel variable: 32
// ADC CHANNEL 9,  value for channel variable: 33
// ADC CHANNEL 10, value for channel variable: 34
// ADC CHANNEL 11, value for channel variable: 35
// ADC CHANNEL 12, value for channel variable: 36
// ADC CHANNEL 13, value for channel variable: 37
// ADC CHANNEL 14, value for channel variable: 38
// ADC CHANNEL 15, value for channel variable: 39
