**USB Interfaces (2):**
- Interface 0 (Mouse)
- - Endpoint 0 (Control Endpoint/Send Mouse Config)
- - Endpoint 1 (Mouse Movements and Button Press)
- Interface 1 (Keyboard)
- - Endpoint 2 (Receive key input and mouse button press for buttons binded to DPI/Key Input)

# Send Config
Endpoint: 0

## Send mouse button binding
```
07 10 BT AT 00 00 00 00
      ^^ ^^
```

**BT** is the physical button that is to be rebinded:
<table>
  <tr>
    <th>Value</th>
    <th>Button Name</th>
  </tr>
  <tr>
    <td>01</td><td>Left Mouse Button</td>
  </tr>
  <tr>
    <td>02</td><td>Right Mouse Button</td>
  </tr>
  <tr>
    <td>03</td><td>Middle Mouse Button</td>
  </tr>
  <tr>
    <td>05 (04 for A704?)</td><td>Side Forward (Towards front)</td>
  </tr>
  <tr>
    <td>04 (05 for A704?)</td><td>Side Backward (Towards user)</td>
  </tr>
  <tr>
    <td>06</td><td>Middle backward (Towards user)</td>
  </tr>
  <tr>
    <td>08</td><td>Middle forward (Towards front)</td>
  </tr>
</table>

**AT** is the rebinded action:
<table>
  <tr>
    <th>Value</th>
    <th>Action Name</th>
  </tr>
  <tr>
    <td>00</td><td>Key Input Action</td>
  </tr>
  <tr>
    <td>01</td><td>Left Click</td>
  </tr>
  <tr>
    <td>02</td><td>Middle Click</td>
  </tr>
  <tr>
    <td>03</td><td>Right Click</td>
  </tr>
  <tr>
    <td>04</td><td>Back</td>
  </tr>
  <tr>
    <td>05</td><td>Forward</td>
  </tr>
  <tr>
    <td>06</td><td>DPI Cycle</td>
  </tr>
  <tr>
    <td>07</td><td>Show Desktop</td>
  </tr>
  <tr>
    <td>08</td><td>Double Click</td>
  </tr>
  <tr>
    <td>09</td><td>Fire</td>
  </tr>
  <tr>
    <td>0a</td><td>DPI+</td>
  </tr>
  <tr>
    <td>0b</td><td>DPI-</td>
  </tr>
</table>

## Send Scroll Wheel Configuration
```
07 11 SC DB 00 00 00 00
      ^^ ^^
```

**SC** is the Scroll Mode:
<table>
  <tr>
    <th>Value</th>
    <th>Mode</th>
  </tr>
  <tr>
    <td>00</td><td>Normal scrolling</td>
  </tr>
  <tr>
    <td>01</td><td>Control Volume</td>
  </tr>
  <tr>
    <td>02</td><td>Disable Scrolling/Debug Mode</td>
  </tr>
</table>


**DB** is the debug value (**SC** must be set to 02 to work):  
<table>
  <tr>
    <th>Value</th>
    <th>Action</th>
  </tr>
  <tr>
    <td>01</td><td>Normal scrolling</td>
  </tr>
  <tr>
    <td>02</td><td>Normal scrolling, direction reversed</td>
  </tr>
  <tr>
    <td>03</td><td>Control volume</td>
  </tr>
  <tr>
    <td>04</td><td>Control volume, direction reversed</td>
  </tr>
  <tr>
    <td>06</td><td>Cycle active DPI Profile</td>
  </tr>
  <tr>
    <td>07</td><td>Trigger the <strong>Show Desktop</strong> action</td>
  </tr>
  <tr>
    <td>08</td><td>Toggle RGB Light (On/Off)</td>
  </tr>
  <tr>
    <td>09</td><td>Flash RGB Light</td>
  </tr>
  <tr>
    <td>0b</td><td>Resets the RGB Light breathing animation, starting with Purple.</td>
  </tr>
  <tr>
    <td>0c</td><td>Resets the RGB Light breathing animation, starting with Blue.</td>
  </tr>
  <tr>
    <td>0d</td><td>Spam <code>z</code> key input!</td>
  </tr>
  <tr>
    <td>10</td><td>Flash RGB Light (In Red color)</td>
  </tr>
  <tr>
    <td>11</td><td>Momentarily turn off the RGB Light, then turn it back on again</td>
  </tr>
  <tr>
    <td>12</td><td>Flash RGB Light (In Blue color)</td>
  </tr>
  <tr>
    <td>13</td><td>Flash RGB Light (In Magenta color)</td>
  </tr>
</table>

## Send Fire Speed Configuration
```
07 12 00 FR 00 00 00 00
         ^^
```
**FR** is a value between **00** and **FF**, from clicking every 8ms to 2s

## Send RGB Light Configuration
```
07 13 7f CF 00 00 00 00
         ^^
```
**CF** is the RGB Configuration:  
<table>
  <tr>
    <th>Value</th>
    <th>RGB Mode</th>
  </tr>
  <tr>
    <td>10</td><td>Slow (6s)</td>
  </tr>
  <tr>
    <td>11</td><td>Slower (5s)</td>
  </tr>
  <tr>
    <td>12</td><td>Slightly Slower (4s)</td>
  </tr>
  <tr>
    <td>13</td><td>Fast (3s)</td>
  </tr>
  <tr>
    <td>14</td><td>Faster (2s)</td>
  </tr>
  <tr>
    <td>15</td><td>Very Fast (1s)</td>
  </tr>
  <tr>
    <td>16</td><td>Static</td>
  </tr>
  <tr>
    <td>17</td><td>Off</td>
  </tr>
</table>

## Send DPI Configuration
```
07 09 AD DP EP 00 00 00
      ^^ ^^ ^^
```

**AD** is the currently active DPI Profile (00 - 03)  
**DP** is the DPI Configuration for the profile, where:
- The left 4 bits is the DPI value from 1-f
- The right 4 bits is the DPI profile (8-b for 4 profiles in order)  
**EP** is the enabled DPI profile, where:
- The left 4 bits is unused
- The right 4 bits is which DPI profile got enabled

## Send Multimedia Key Input
```
07 17 MK MK 00 00 00 00
      ^^ ^^
```  

**MK** is a 2 byte value representing the key to be pressed ([HID Usage Table](https://usb.org/sites/default/files/hut1_5.pdf) Consumer Page):
- `00 ea` - Music Vol -
- `00 e9` - Music Vol +
- `02 23` - Open Web Browser
- `01 83` - Open Windows Media Player (Settings on KDE)
- `01 8a` - Open Mail Client
- `00 b6` - Last Song
- `00 b5` - Next Song
- `00 e2` - Mute Sound Volume
- `00 cd` - Play/Pause
- `01 92` - Open Calculator

## Send HID Keyboard Input
```
07 15 KY KY KY KY KY KY
```

Can have a maximum of 6 key held.  
**KY** is the [HID Keyboard Keycode](https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2)

**Notes:**
1. Give a bit of delay between each key command
2. Remember to release the key after use by setting all **KY** to 00

# Receive Mouse Data
Endpoint: 2

## On Button Click
Note: Only sent if button is binded to **DPI+**, **DPI-**, **DPI Cycle** or **Key Input Action**  

```
06 BT DP
   ^^ ^^
```

**BT** is the physical button pressed (Not the binded action):

<table>
  <tr>
    <th>Value</th>
    <th>Button Name</th>
  </tr>
  <tr>
    <td>f1</td><td>Left Mouse Button</td>
  </tr>
  <tr>
    <td>f2</td><td>Middle Mouse Button</td>
  </tr>
  <tr>
    <td>f3</td><td>Right Mouse Button</td>
  </tr>
  <tr>
    <td>f4</td><td>Side Backward (Towards user)</td>
  </tr>
  <tr>
    <td>f5</td><td>Side Forward (Towards front)</td>
  </tr>
  <tr>
    <td>f6</td><td>Middle backward (Towards user)</td>
  </tr>
  <tr>
    <td>f8</td><td>Middle forward (Towards front)</td>
  </tr>
</table>

**DP** is the DPI profile that should be active (00-03)

## On Multimedia Key input
```
02 KY KY
   ^^ ^^
```

**KY** Multimedia key inputted by the mouse, see [HID Usage Table](https://usb.org/sites/default/files/hut1_5.pdf) Consumer Page