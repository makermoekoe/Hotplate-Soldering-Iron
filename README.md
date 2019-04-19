# Instagram-Stats-Oled

<a href="https://www.instagram.com/p/BsdeiV0oKy9/?utm_source=ig_web_button_share_sheet"><img src="docs/oled.png" width="300px"></a>

(Click on the image for a small video.)

This is just a 5 minute project for the Instagram lovers.
To build this tiny notification display you just need two things:
- a Wemos D1 mini
- an oled shield for Wemos D1 mini

Stack them together and connect it to your pc with a micro usb cable.

Before you upload the sketch to the ESP, download and install the [InstagramStats](https://github.com/witnessmenow/arduino-instagram-stats) library. Remember that you have to install the [JSON streaming parser](https://github.com/squix78/json-streaming-parser) as well, but this could be done easier via the Arduino library manager.

Moreover you have to install libraries for the oled display. If not already done, you can search for [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) and [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) inside the library manager and install it from there.

Last step is to add your wifi credentials to the following lines of the sketch and to enter your's or anybody's Instagram username.

```
char ssid[] = "XXX";       // your network SSID (name)
char password[] = "XXX";  // your network key
```
```
String userName = "maker.moekoe";
```
