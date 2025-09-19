# Instant A704/A704F Mouse Utilities for Linux
![](https://img.shields.io/badge/jank_inside-brown) ![](https://img.shields.io/badge/works_on-my_machine-green)

This is a mouse configuration tools for the [A704](https://instant-sys.com/uploads/pdf/norm/SPEC/A704C_SPEC_EN.V1.01.pdf)/[A704F](https://instant-sys.com/uploads/pdf/norm/SPEC/A704F_SPEC_EN.V1.00.pdf) Gaming Mouse IC by Instant Microelectronics.  

Configurable items including custom button bindings, change speed of breathing light and DPI configuration.  

<table>
    <tr>
        <td><img src="./assets/1.png"></td>
        <td><img src="./assets/2.png"></td>
        <td><img src="./assets/3.png"></td>
    </tr>
</table>

Vendor IDs/Product IDs:
- `18f8:1286` (A704F)
- `30fa:1701` (A704)
All the observed details can be found in [SPECS.md](./SPECS.md)

For more context and story, [I have written a blog about it](https://blog.lx862.com/blog/2024-05-13-reverse-engineering-a-mouse/)

## Installation & Usage
**For Debian-based users:**
- Download the deb from the [release page](https://github.com/Kenny-Hui/Instant-A704F-Mouse-Utilities/releases/latest) and install it.

**For other distribution:**
Sorry I do not know how to or I am too lazy to package.

0. Make sure you have Qt 6 installed. (If you are running KDE Plasma 6.x then you already have Qt 6)
1. Download the binary from the [release page](https://github.com/Kenny-Hui/Instant-A704F-Mouse-Utilities/releases/latest)
2. Put it in an safe location
3. Run it!

**Note:**
The **Side Button A** and **Side Button B** is intentionally named ambigiously as their meaning may not be the same on some mouse models (A means forward button and B means backward button, however some mouse model have it the other way around). You will have to do some basic testing to figure that out for your own mouse.

### Flags
`--apply` - Apply settings to the mouse unattendedly without launching a GUI (As the mouse does not store settings persistently)  
`--daemon` - Start a background daemon, this is required to handle key input functionalities.

A common setup is to add an autostart entry with the program argument `--apply --daemon` so that it applies the mouse settings on startup and run persistently in the background.

## Build from Source
```
git clone https://github.com/AmberIsFrozen/Instant-A704F-Mouse-Utilities
cd Instant-A704F-Mouse-Utilities
mkdir build
cd build
cmake ..
make
```

To install:
```
make install
```

## License
This project is licensed under the MIT License.