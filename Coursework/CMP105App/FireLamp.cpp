#include "FireLamp.h"

FireLamp::FireLamp()
{
	addLampAnimFrames();
}

FireLamp::~FireLamp()
{

}

void FireLamp::update(float dt)
{

}

void FireLamp::addLampAnimFrames()
{
	for (int i = 0; i < (lamp_1_frames * LAMPS_FRAME_WIDTH); i += LAMPS_FRAME_WIDTH)
	{
		lamp_1.addFrame(sf::IntRect(i, 0, LAMPS_FRAME_WIDTH, LAMPS_FRAME_HEIGHT));
	}

	for (int i = 0; i < (lamp_2_frames * LAMPS_FRAME_WIDTH); i += LAMPS_FRAME_WIDTH)
	{
		lamp_2.addFrame(sf::IntRect(256 + i, 0, LAMPS_FRAME_WIDTH, LAMPS_FRAME_HEIGHT));
	}

	for (int i = 0; i < (lamp_3_frames * LAMPS_FRAME_WIDTH); i += LAMPS_FRAME_WIDTH)
	{
		lamp_3.addFrame(sf::IntRect(512 + i, 0, LAMPS_FRAME_WIDTH, LAMPS_FRAME_HEIGHT));
	}

	for (int i = 0; i < (lamp_4_frames * LAMPS_FRAME_WIDTH); i += LAMPS_FRAME_WIDTH)
	{
		lamp_4.addFrame(sf::IntRect(768 + i, 0, LAMPS_FRAME_WIDTH, LAMPS_FRAME_HEIGHT));
	}

	lamp_1.setFrameSpeed(1.0f / 10.f);
	lamp_2.setFrameSpeed(1.0f / 20.f);
	lamp_3.setFrameSpeed(1.0f / 20.f);
	lamp_4.setFrameSpeed(1.0f / 20.f);
}

Animation* FireLamp::getLamp_1()
{
	return &lamp_1;
}

Animation* FireLamp::getLamp_2()
{
	return &lamp_2;
}

Animation* FireLamp::getLamp_3()
{
	return &lamp_3;
}

Animation* FireLamp::getLamp_4()
{
	return &lamp_4;
}