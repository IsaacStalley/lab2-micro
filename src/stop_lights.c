#include <avr/io.h>
#include <avr/interrupt.h>

// State Machine states
#define TRAFFIC_GREEN_STATE 0
#define TRAFFIC_BLINK_STATE 1
#define TRANSITION_TO_CROSSWALK_STATE 2
#define CROSSWALK_GREEN_STATE 3
#define CROSSWALK_BLINK_STATE 4
#define TRANSITION_TO_TRAFFIC_STATE 5

typedef unsigned int uint;

volatile uint buttonPressed = 0; // Flag to indicate button press
volatile uint timerCounter = 0; // Timer counter to track time

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
            break;
        // TRAFFIC_BLINK_STATE
        case TRAFFIC_BLINK_STATE:
            break;
        // TRANSITION_TO_CROSSWALK_STATE
        case TRANSITION_TO_CROSSWALK_STATE:
            break;
        // CROSSWALK_GREEN_STATE
        case CROSSWALK_GREEN_STATE:
            break;
        // CROSSWALK_BLINK_STATE
        case CROSSWALK_BLINK_STATE:
            break;
        // TRANSITION_TO_TRAFFIC_STATE
        case TRANSITION_TO_TRAFFIC_STATE:

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

    while (1) {
        updateStateMachine();
    }
}