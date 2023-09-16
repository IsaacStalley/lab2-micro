#include <avr/io.h>
#include <avr/interrupt.h>

// State Machine states
#define TRAFFIC_GREEN_STATE 0
#define TRAFFIC_BLINK_STATE 1
#define TRANSITION_TO_CROSSWALK_STATE 2
#define CROSSWALK_GREEN_STATE 3
#define CROSSWALK_BLINK_STATE 4
#define TRANSITION_TO_TRAFFIC_STATE 5

// Output Pins (LEDS)
#define TRAFFIC_GREEN 1 // PB1
#define TRAFFIC_RED 2 // PB2
#define CROSSWALK_GREEN 3 //PB3
#define CROSSWALK_GREEN2 4 //PB4
#define CROSSWALK_RED 5 // PB5
#define CROSSWALK_RED2 6 // PB6

#define BLINK_COUNTER 6 // Number of LED blinks

typedef unsigned int uint;

volatile uint buttonPressed = 0; // Flag to indicate button press
volatile uint timerCounter = 0; // Timer counter to track time
uint trafficMinimumMet = 0;
uint current_state = TRAFFIC_GREEN_STATE;

ISR(INT0_vect) { // INTO button interrupt
    buttonPressed = 1; // Button flag
}

ISR(TIMER0_COMPA_vect) { // Timer compare interrupt
    timerCounter++; // Timer counter
}

void delay(float seconds) { // Delay function using timer
    TCNT0 = 0; // Reset the timer
    timerCounter = 0; // Reset counter
    while (timerCounter < 30*seconds) {}
}

void updateStateMachine() {
    switch (current_state) {
        // TRAFFIC_GREEN_STATE 
         case TRAFFIC_GREEN_STATE:
            PORTB &= ~(1 << TRAFFIC_RED); // Turn off 
            PORTB |= (1 << TRAFFIC_GREEN) | (1 << CROSSWALK_RED) | (1 << CROSSWALK_RED2); // Turn on
            if (!trafficMinimumMet) { // Check for minimum time met of 10s
                delay(10);
                trafficMinimumMet = 1; // Minimum time met flag
            }
            else if (buttonPressed) { // If minumum time met and button pressed
                current_state = TRAFFIC_BLINK_STATE;
                buttonPressed = 0;
                trafficMinimumMet = 0;
            }
            break;
        // TRAFFIC_BLINK_STATE
        case TRAFFIC_BLINK_STATE:
            for (int i = 0; i < BLINK_COUNTER; i++) { // For loop to blink green traffic light
                PORTB ^= (1 << TRAFFIC_GREEN); // Toggle green traffic light
                delay(0.5);
            }
            current_state = TRANSITION_TO_CROSSWALK_STATE;
            break;
        // TRANSITION_TO_CROSSWALK_STATE
        case TRANSITION_TO_CROSSWALK_STATE:
            PORTB &= ~(1 << TRAFFIC_GREEN); // Turn off
            PORTB |= (1 << TRAFFIC_RED); // Turn on
            delay(1);
            current_state = CROSSWALK_GREEN_STATE;
            break;
        // CROSSWALK_GREEN_STATE
        case CROSSWALK_GREEN_STATE:
            buttonPressed = 0;
            PORTB &= ~((1 << CROSSWALK_RED) | (1 << CROSSWALK_RED2)); // Turn off
            PORTB |= (1 << CROSSWALK_GREEN) | (1 << CROSSWALK_GREEN2); // Turn on
            delay(10);
            current_state = CROSSWALK_BLINK_STATE;
            break;
        // CROSSWALK_BLINK_STATE
        case CROSSWALK_BLINK_STATE:
                for (int i = 0; i < BLINK_COUNTER; i++) { // Loop to blink crosswalk green light
                    PORTB ^= (1 << CROSSWALK_GREEN) | (1 << CROSSWALK_GREEN2); // Toggle
                    delay(0.5);
                }
            current_state = TRANSITION_TO_TRAFFIC_STATE;
            break;
        // TRANSITION_TO_TRAFFIC_STATE
        case TRANSITION_TO_TRAFFIC_STATE:
            PORTB &= ~((1 << CROSSWALK_GREEN) | (1 << CROSSWALK_GREEN2)); // Turn off
            PORTB |= (1 << CROSSWALK_RED) | (1 << CROSSWALK_RED2); // Turn on
            delay(1);
            current_state = TRAFFIC_GREEN_STATE;
            break;
    }
}

int main() {
    GIMSK |= (1 << INT0); // Enable INT0
    MCUCR |= (1 << ISC01); // Trigger on falling edge
    sei(); // Enable global interrupts

    TCCR0A |= (1 << WGM01); // Set CTC mode (WGM02 is in TCCR0B)
    TIMSK |= (1 << OCIE0A); // Enable timer compare interrupt
    TCCR0B |= (1 << CS02) | (1 << CS00); // Start timer with prescaler 1024
    OCR0A = 255; // Set compare match value for desired delay

    DDRD &= ~(1 << PD2); // Set D2 as input pin
    DDRB |= (1 << TRAFFIC_GREEN) | (1 << TRAFFIC_RED) | (1 << CROSSWALK_GREEN) | (1 << CROSSWALK_GREEN2) | (1 << CROSSWALK_RED) | (1 << CROSSWALK_RED2); // Set pins as outputs
    PORTB = 0;

    while (1) {
        updateStateMachine();
    }
}