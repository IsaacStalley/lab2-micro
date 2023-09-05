#include <pic14/pic12f683.h>

#define GREEN_STATE 0;
#define RED_STATE 1;
#define YELLOW_STATE 2;

struct stateMachine {
    int current_state;
    // We can add more variables needed here.
};

void transitionToGreen(struct stateMachine sM) {
    sM.current_state = GREEN_STATE;
}

void transitionToYellow(struct stateMachine sM) {
    sM.current_state = YELLOW_STATE;
}

void transitionToRed(struct stateMachine sM) {
    sM.current_state = RED_STATE;
}

// Automatically transitions the state machine to the next state
void updateStateMachine(struct stateMachine sM) {
    switch (sM.current_state) {
        case RED_STATE:
            transitionToGreen(sM);
            break;
        case YELLOW_STATE:
            transitionToRed(sM);
            break;
        case GREEN_STATE:
            transitionToYellow(sM);
            break;
        default:
            break;
    }
}

int main() {
    struct stateMachine sM;
    transitionToRed(sM);

    while (1) {
        // logic here, for example:
        // if (sM.current_state == yellow)
        // delay(time)
        // updateStateMachine(sM)
        
    }
}