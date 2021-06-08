#include "main.h"
// #include "OLEDDisplay.h"
void Display::PrintString(String text, bool preclear, bool render, const uint8_t* font, int16_t x, int16_t y, OLEDDISPLAY_TEXT_ALIGNMENT align) {
	if (preclear)
		this->clear();
	setTextAlignment(align);
	setFont(font);
	drawString(x, y, String(text));
	if (render)
		this->display();
}

Display::Display(uint8_t _address, int _sda, int _scl, OLEDDISPLAY_GEOMETRY g): SSD1306Wire(_address, _sda, _scl, g)
{
};
