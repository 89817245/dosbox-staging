/*
 *  Copyright (C) 2002-2021  The DOSBox Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef DOSBOX_JOYSTICK_H
#define DOSBOX_JOYSTICK_H

#include "dosbox.h"

void JOYSTICK_Enable(uint8_t which, bool enabled);

void JOYSTICK_Button(uint8_t which, int num, bool pressed);

// takes in the joystick axis absolute value from -32768 to 32767
void JOYSTICK_Move_X(uint8_t which, int16_t x_val);
void JOYSTICK_Move_Y(uint8_t which, int16_t y_val);

bool JOYSTICK_IsEnabled(uint8_t which);

bool JOYSTICK_GetButton(uint8_t which, int num);

// returns a percentage from -1.0 to +1.0 along the axis
double JOYSTICK_GetMove_X(uint8_t which);
double JOYSTICK_GetMove_Y(uint8_t which);

void JOYSTICK_ParseConfiguredType();

enum JoystickType {
	JOY_UNSET,
	JOY_DISABLED, // joystick subsystem is fully disabled (won't even query)
	JOY_NONE,     // joystick subsystem will be available for mapping only
	JOY_AUTO,
	JOY_2AXIS,
	JOY_4AXIS,
	JOY_4AXIS_2,
	JOY_FCS,
	JOY_CH
};

extern JoystickType joytype;
extern bool button_wrapping_enabled;

#endif
