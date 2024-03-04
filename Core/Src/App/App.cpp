/*
 * App.cpp
 *
 *  Created on: Jan 29, 2024
 *      Author: user
 */

#include <App/App.hpp>


App::App(){

}


void App::init(){
	led1.init();

	led1.blink(100, 500);


}
void App::loop(){
	led1.iterate();


}

