#include "SFOutput.hpp"



//HARDWARE

bool SFOutput::pinStateRead(){
	return ((uint8_t) HAL_GPIO_ReadPin(port, pin)) ^ (uint8_t) logic ;
}

uint32_t SFOutput::getTick(){
	return System::getTick();
}

void SFOutput::pinStateWrite(uint8_t state){
	HAL_GPIO_WritePin(port, pin, (GPIO_PinState)(state ^ (uint8_t) logic ));
}






SFOutput::SFOutput(GPIO_TypeDef* port, uint32_t pin, Logic logic) :
		port(port), pin(pin), logic(logic){

	this->state = false;

	off_time = 0;
	on_time = 0;
	procedure = Procedure::MANUAL;
	lastiterate = 0;
	cycle = Cycle::ON;

	numberblink = -1;

	sequence = 0;
	sequence_number = 0;
	sequence_last_iterate = 0;
	sequence_position = 0;
	sequence_time_to_go = 0;
}

void SFOutput::init(){
	set(false);
}



bool SFOutput::get(){
	return state;
}

bool SFOutput::read(){
	return pinStateRead();
}

void SFOutput::set(bool state){
	pinStateWrite(this->state);
}
void SFOutput::ON(){
	procedure = Procedure::MANUAL;
	set(true);
}
void SFOutput::OFF(){
	procedure = Procedure::MANUAL;
	set(false);
}
void SFOutput::toggle(){
	this->state = (state) ? false : true;
	set(!pinStateRead());
}

void SFOutput::blink(uint32_t time, Cycle cycl){
	blink(time, time, cycl);
}
void SFOutput::blink(uint32_t on_time, uint32_t off_time, Cycle cycl){
	if(procedure == Procedure::BLINKING && this->on_time == on_time && this->off_time == off_time)
		return;

	procedure = Procedure::BLINKING;
	this->on_time = on_time;
	this->off_time = off_time;
	cycle = cycl;
	numberblink = -1;
	set((bool) cycl);
	lastiterate = getTick();
}

void SFOutput::blinkNumber(int number, uint32_t time, Cycle cycl){
	blinkNumber(number, time, time, cycl);
}
void SFOutput::blinkNumber(int number, uint32_t on_time, uint32_t off_time, Cycle cycl){
	procedure = Procedure::NUMBER_BLINK;
	this->on_time = on_time;
	this->off_time = off_time;
	cycle = cycl;
	numberblink = (number * 2) - 1;
	if(cycl == Cycle::OFF)
		numberblink++;
	set((bool) cycl);
	lastiterate = getTick();
}

void SFOutput::timeON(uint32_t timeon){
	timeSET(timeon, 1);
}
void SFOutput::timeOFF(uint32_t timeoff){
	timeSET(timeoff, 0);
}
void SFOutput::timeTOG(uint32_t time){
	if(get())
		timeSET(time, 0);
	else
		timeSET(time, 1);
}

void SFOutput::timeSET(uint32_t time, bool startvalue){
	procedure = Procedure::NUMBER_BLINK;
	numberblink = 1;
	if(startvalue){
		cycle = Cycle::ON;
		on_time = time;
	}else{
		cycle = Cycle::OFF;
		off_time = time;
	}
	set((bool) cycle);
	lastiterate = getTick();
}

SFOutput::Procedure SFOutput::getMode(){
	return procedure;
}

void SFOutput::playSequence(Sequence* sequnce, uint16_t number){
	procedure = Procedure::Sequence;
	this->sequence = sequnce;
	this->sequence_number = number;
	this->sequence_last_iterate = 0;
	this->sequence_position = 0;
	this->sequence_time_to_go = 0;
}
void SFOutput::stopSequence(){
	if(procedure == Procedure::Sequence)
		procedure = Procedure::MANUAL;
}

void SFOutput::iterate(){
	uint32_t TICK = getTick();

	if(procedure == Procedure::BLINKING || procedure == Procedure::NUMBER_BLINK){
		if(cycle == Cycle::ON){
			if(on_time <= (TICK - lastiterate)){  // if(time>= (lastiterate+on_time)){
				set(0);
				cycle = Cycle::OFF;
				lastiterate = TICK;
				numberblink--;
				if(procedure == Procedure::NUMBER_BLINK && numberblink == 0)
					procedure = Procedure::MANUAL;
				return;
			}
		}else if(cycle == Cycle::OFF){
			if(off_time <= (TICK - lastiterate)){  // if(time >= (lastiterate + off_time)){
				set(1);
				cycle = Cycle::ON;
				lastiterate = TICK;
				numberblink--;
				if(procedure == Procedure::NUMBER_BLINK && numberblink == 0)
					procedure = Procedure::MANUAL;
				return;
			}
		}
	}else if(procedure == Procedure::Sequence){
		if(sequence_time_to_go <= (TICK - sequence_last_iterate)){
			sequence_last_iterate = TICK;

			if(sequence->section[sequence_position].state)
				set(1);
			else
				set(0);
			sequence_time_to_go = sequence->section[sequence_position].time;

			sequence_position++;
			if(sequence_position == sequence->length){
				sequence_position = 0;
				if(sequence_number > 0){
					sequence_number--;
					if(sequence_number == 0){
						// set(0);
						procedure = Procedure::MANUAL;
					}
				}
			}
		}
	}
}
