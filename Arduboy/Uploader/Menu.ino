unsigned char const menu1[] PROGMEM  = "Browse Collection";
unsigned char const menu2[] PROGMEM  = "Clock";
unsigned char const menu3[] PROGMEM  = "Online update repo";
unsigned char const menu4[] PROGMEM  = "Reinstall dock app";
unsigned char const menu5[] PROGMEM  = "Power off dock";
unsigned char* const menu[] PROGMEM = { menu1, menu2, menu3, menu4, menu5 };

void SwitchToTransfer()
{
  //tunes.tone(1000, 60);
  currentTransferAnimationFrame = 0;
  currentMode = TRANSFER;
  nextEvent = millis() + 10000;
}

void doMenu()
{
  arduboy.println("DOCK MENU V08");

  if (arduboy.justReleased(DOWN_BUTTON))
  {
    //tunes.tone(1100, 30);
    selectedItem++;
  }

  if (arduboy.justReleased(UP_BUTTON))
  {
    //tunes.tone(1100, 30);
    selectedItem--;
  }

  selectedItem = selectedItem < 0 ? maximumItems - 1 : (selectedItem >= maximumItems ? 0 : selectedItem);

  for (byte i = 0; i < maximumItems; i++)
  {
    arduboy.print(selectedItem == i ? F("->") : F("  "));
    const char * menuItem = reinterpret_cast<const char *>(pgm_read_word(&(menu[i]))); // Points to
    arduboy.println(reinterpret_cast<const __FlashStringHelper*>(menuItem));
  }

  if (arduboy.justReleased(A_BUTTON))
  {
    switch (selectedItem)
    {
      case 0:
        currentMode = REPOINIT;
        break;

      case 1:
        currentMode = CLOCKINIT;
        break;

      case 2:
        Serial.print(F("<REPOUPDATE>"));
        repoTotalGames = -1;
        nextEvent = millis() + 10000;
        currentMode = REPOUPDATE;
        break;

      case 3:
        Serial.print(F("<UPDATE>"));
        SwitchToTransfer();
        break;

      case 4:
        Serial.print(F("<SHUTDOWN>"));
        nextEvent = millis() + 5000;
        currentMode = SHUTDOWN;
        break;
    }
  }
}
