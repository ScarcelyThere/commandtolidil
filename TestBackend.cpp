/* TestBackend.cpp: Testing Backend for further DeskJet3600 and Pen tests
 * Copyright (C) 2023 Scarcely There.
 *
 * dj3600command is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any other version.
 *
 * dj3600command is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the the GNU General Public License along with
 * dj3600command. If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <chrono>
#include <random>
#include "TestBackend.hpp"

TestBackend::TestBackend () = default;
TestBackend::~TestBackend () = default;

bool
TestBackend::getDeviceID (std::string& deviceID)
{
    std::chrono::time_point rightNow = std::chrono::system_clock::now ();
    std::ranlux24 randomGenerator (std::chrono::system_clock::to_time_t(rightNow));
    std::uniform_int_distribution randomNumber (0,2);

    int testCase = randomNumber (randomGenerator);

    deviceID = testCases[testCase];
    std::cerr << "ERROR: " << testDescriptions[testCase] <<  std::endl;

    return true;
}

// vim: et sw=4
