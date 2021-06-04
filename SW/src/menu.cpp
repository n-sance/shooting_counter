#include "main.h"


void startCount(Ticker &time1)
{
	time1.start();
	Serial.println("INFO: Start count mode On");
};

void someAnother(void)
{
	Serial.println("INFO: Start some another mode On");
};

MenuItem *init_menu(void (*function1)(), void (*function2)()) {
	MenuItem *first = new MenuItem();
	MenuItem *second = new MenuItem();
	first->title = "Debug";
	first->trigger_on_btn = function1;
	first->next = second;
	second->title = "Normal";
	second->trigger_on_btn = function2;
	second->next = first;
	return first;
};

MenuItem *getNext(MenuItem *current) {
	MenuItem *result = new MenuItem();
	result = current->next;
	return result;
};

