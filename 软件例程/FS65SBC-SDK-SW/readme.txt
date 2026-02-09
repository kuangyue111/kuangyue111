/*
 * Copyright (c) 2016 - 2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
Version     Comment
1.0         Initial release.
1.1			Example for the Cobra MCU added.

*******
Content
*******

This archive contains embedded generic driver for the FS65 SBC family and 
following examples:
 - S32 Design Studio (version 2018.R1) for S32K144 MCU.
 - S32 Design Studio for Power Architecture (version 2017.R1) for MPC5744P EVB (Panther MCU).
 - S32 Design Studio for Power Architecture (version 2017.R1) for MPC5777C EVB (Cobra MCU).

The purpose of this example is to demonstrate usage of the driver API. It contains Fail-safe 
initialization, Register R/W, Watchdog refresh and settings, MUX_OUT selection and FS0b release
functions.


Following directories are present in this archive:

doc
---
Contains driver documentation. 
- FS65 Programmers Guide.pdf
- FS65 FS45 User Manual.pdf

FS65_driver
-----------
The driver itself.

FS65_S32K_Example
--------------------
Design Studio project for the S32K144EVB-Q100 board.

FS65_Panther_Example
--------------------
Design Studio project for the DEVKIT-MPC5744P board. Can be used with other boards
but PIN mux must be reviewed in this case.
Also UART console output must be disabled if the UART is not present on the board.

FS65_Cobra_Example
------------------
Design Studio project for the MPC5777C-x16DS board. Can be used with other boards
but PIN mux must be reviewed in this case.
Also UART console output must be disabled if the UART is not present on the board.