/**
 * MK4duo Firmware for 3D Printer, Laser and CNC
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2013 Alberto Cotronei @MagoKimbra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../../MK4duo.h"

#if HAS_CASE_LIGHT

  CaseLight caselight;

  uint8_t CaseLight::brightness = CASE_LIGHT_DEFAULT_BRIGHTNESS;
  bool    CaseLight::status     = CASE_LIGHT_DEFAULT_ON;

  #if ENABLED(CASE_LIGHT_USE_NEOPIXEL)
    LEDColor case_light_color = CASE_LIGHT_NEOPIXEL_COLOR;
  #endif

  void CaseLight::update() {

    const uint8_t onoff     = status ? brightness : 0,
                  intensity = INVERT_CASE_LIGHT ? 255 - onoff : onoff;

    #if ENABLED(CASE_LIGHT_USE_NEOPIXEL)

      leds.set_color(
        MakeLEDColor(case_light_color.r, case_light_color.g, case_light_color.b, case_light_color.w, intensity),
        false
      );

    #else // !CASE_LIGHT_USE_NEOPIXEL

      if (USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN))
        HAL::analogWrite(CASE_LIGHT_PIN, intensity);
      else {
        const bool s = status ? !INVERT_CASE_LIGHT : INVERT_CASE_LIGHT;
        HAL::digitalWrite(CASE_LIGHT_PIN, s ? HIGH : LOW);
      }

    #endif // !CASE_LIGHT_USE_NEOPIXEL
  }

  void CaseLight::report() {

    SERIAL_SM(ECHO, "Case light:");

    if (status) {
      if (USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN)) SERIAL_EV((int) brightness);
      else SERIAL_EM("on");
    }
    else SERIAL_EM("off");
  }

#endif // HAS_CASE_LIGHT
