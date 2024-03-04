/*
 * App.hpp
 *
 *  Created on: Jan 29, 2024
 *      Author: user
 */

#ifndef INC_APP_APP_HPP_
#define INC_APP_APP_HPP_

#include <main.h>
#include <SFOutput/SFOutput.hpp>

class App {

public:
	App();
	void init();
	void loop();


private:
	SFOutput led1 = SFOutput(LED1_GPIO_Port, LED1_Pin);




};

static App app;

extern "C" void appInit(){
	app.init();
}

extern "C" void appLoop(){
	app.loop();
}



#endif /* INC_APP_APP_HPP_ */
