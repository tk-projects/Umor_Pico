/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <RadioLib.h> // Include the RadioLib library
#include "Hal.h"
#include "modules/SX126x/SX1262.h"
#include "modules/SX126x/SX126x.h"
#include "Module.h"


// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 1000
#endif

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

// LoRa SetUp
// include the hardware abstraction layer
#include "PicoHal.h"

// Define SPI pins
#define SPI_PORT spi1
#define SPI_MISO 12  // MISO pin
#define SPI_MOSI 11 //19  // MOSI pin
#define SPI_SCK 10 //18   // SCK pin

// Define LoRa module pins for SX1262
#define CS_PIN 3     // Chip Select
#define DIO1_PIN 20//6   // DIO1 Pin
#define RST_PIN 15 //7    // Reset Pin
#define BUSY_PIN 2 //8   // Busy Pin

// Init of LoRa Pins
void initLoRaPins() {
    // Initialize Chip Select (CS_PIN)
    gpio_init(CS_PIN);
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_put(CS_PIN, 1);  // Set CS high

    // Initialize DIO1 pin
    gpio_init(DIO1_PIN);
    gpio_set_dir(DIO1_PIN, GPIO_IN);  // Set DIO1 as input

    // Initialize Reset pin
    gpio_init(RST_PIN);
    gpio_set_dir(RST_PIN, GPIO_OUT);
    gpio_put(RST_PIN, 1);  // Set Reset high

    // Initialize Busy pin
    gpio_init(BUSY_PIN);
    gpio_set_dir(BUSY_PIN, GPIO_IN);  // Set BUSY as input
}

// Create an instance of the HAL for Pico
PicoHal* hal = new PicoHal(SPI_PORT, SPI_MISO, SPI_MOSI, SPI_SCK);

// Create a Module instance using the HAL object
Module mod = Module(hal, CS_PIN, DIO1_PIN, RST_PIN, BUSY_PIN);

// Initialize the LoRa radio (SX1262) with the Module
SX1262 lora = SX1262(&mod);

int main() {
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    // Initial Blinking to indicate startup
    pico_set_led(true);
    sleep_ms(200);
    pico_set_led(false);
    sleep_ms(200);
    pico_set_led(true);
    sleep_ms(200);
    pico_set_led(false);
    sleep_ms(200);
    pico_set_led(true);
    sleep_ms(200);
    pico_set_led(false);
    
    // Initialise USB Communication
    stdio_init_all();

    sleep_ms(3000);
    printf("\nInitialising Pico ");
    sleep_ms(300);
    printf(".");
    sleep_ms(300);
    printf(".");
    sleep_ms(300);
    printf(".");
    sleep_ms(500);


    // Initialise LoRa

    // Initialize LoRa pins
    initLoRaPins();
    // Start LoRa communication
    printf("\nNow intialising LoRa Communication");

    int state = lora.begin(868.0, 125.0, 9, 7, 0x12, 17);

    if (state == RADIOLIB_ERR_NONE) {
        printf("\nLoRa initialized successfully!\n");
    } else {
        printf("\nFailed to initialize LoRa, code: %d\n", state);
        while (true);
    }


    while (true) {
        pico_set_led(true);
        printf("\nLED On");
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        printf("\nLED Off");
        sleep_ms(LED_DELAY_MS);
    }
}