long interval = 10000;  // 10 secs
long lastServerCheck = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  if (toggleButtonChanged) {
    makeServerCallForOtherNeighbor();
  }

  // periodic check of server:
  if (millis() - lastServerCheck > interval) {
    // turn on a warning LED to say that you're busy
    serverCheck();
    lastServerCheck = millis();
    // turn off a warning LED to say that you're free again
  }
}

makeServerCallForOtherNeighbor();

serverCheck();



//states for our device
//- just listening to buttons
//- just making a request
//- listening to buttons while I wait for a response
