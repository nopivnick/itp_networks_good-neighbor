/*
   Some in-progress code for toggling the device UI (e.g., volume level)
   between my device state and my neighbor's device state.
   
   Example states for each device:
   - just listening to buttons
   - just making a request
   - listening to buttons while I wait for a response
*/

long interval = 10000;  // 10 secs
long lastServerCheck = 0;

// We've run out of digital pins so we need to use an analog as digital
// NOTE: when using analog as digigal it's recommended  to keep as much distance
// from other anolog pins in use inorder to reduce noise / interference
int buttonPeekPin = A6;
boolean buttonPeekState = false;

void setup() {

}

void loop() {

  if (buttonPeekState = true) {
    getNeighborDeviceState();
  }

  // periodic check of server:
  if (millis() - lastServerCheck > interval) {
    // turn on a warning LED to say that you're busy
    serverCheck();
    lastServerCheck = millis();
    // turn off a warning LED to say that you're free again
  }
}

getNeighborDeviceState(); {
}

serverCheck(); {
}
