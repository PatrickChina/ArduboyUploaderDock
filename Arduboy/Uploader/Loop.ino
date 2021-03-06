void loop()
{
  if (Serial.available())
    readSerial();

  if (!(arduboy.nextFrame()))
    return;

  arduboy.pollButtons();

  arduboy.clear();
  arduboy.setTextSize(1);

  if (millis() - lastReceivedPing > (freshBoot ? 2000 : 1000))
  {
    sprites.drawSelfMasked(0, 0, error, currentErrorAnimationFrame);
    if (arduboy.everyXFrames(60)) currentErrorAnimationFrame++;
    if (currentErrorAnimationFrame > 2) currentErrorAnimationFrame = 0;

  }
  else
  {
    ping();
    long tmp;

    switch (currentMode)
    {
      case SHUTDOWN:
        arduboy.println("Please wait, dock is");
        arduboy.print("shutting down...");
        lastReceivedPing = millis();

        if (nextEvent < millis())
        {
          currentMode = WAITING;
          nextEvent = millis() + 10000;
        }
        break;

      case WAITING:
        arduboy.println();
        arduboy.println();
        arduboy.println("It's now safe to turn");
        arduboy.print("off your dock.");
        lastReceivedPing = millis();

        if (nextEvent < millis())
        {
          currentMode = MENU;
        }
        break;

      case REPOINITBUFFER:
        // Fill some game names in the buffer
        if (repoLoaded == 0)
          clearGameNames();
        currentMode = REPO;
        break;

      case REPOUPDATE:
        arduboy.println("Please wait, dock is");
        arduboy.print("updating the repo...");
        lastReceivedPing = millis();

        if (nextEvent < millis())
          currentMode = MENU;
        break;

      case REPOINIT:
        if (repoTotalGames != -1)
          currentMode = REPO;
        else if (getDockInt("<REPOSIZE>", &tmp))
        {
          repoTotalGames = tmp;
          currentMode = REPOINITBUFFER;
        }
        break;

      case CLOCKINIT:
        if (getDockInt("<TIME>", &tmp))
        {
          setTime(tmp);
          currentMode = CLOCK;
        }
        break;

      case REPO:
        doRepo();
        break;

      case CLOCK:
        doClock();
        break;

      case FAIL:
        arduboy.println("Error.");

        if (nextEvent < millis())
          currentMode = MENU;
        break;


      case TRANSFER:
        sprites.drawSelfMasked(0, 0, transfer, currentTransferAnimationFrame);
        if (arduboy.everyXFrames(6)) currentTransferAnimationFrame++;
        if (currentTransferAnimationFrame > 4) currentTransferAnimationFrame = 0;

        if (nextEvent < millis())
        {
          currentMode = FAIL;
          nextEvent = millis() + 2000;
        }
        break;

      case MENU:
        doMenu();
        break;
    }
  }
  arduboy.display();
}

