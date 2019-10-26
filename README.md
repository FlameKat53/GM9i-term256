<p align="center">
	<img src="https://github.com/RocketRobz/GodMode9i/blob/master/resources/logo2_small.png"><br>
	<b>A WIP file browser for (3)DS(i) :godmode:</b>
</p>

GM9i-term256 is a **COMPLETELY WORK IN PROGRESS** file browser for the Nintendo (3)DS(i).


In order to compile this application on your own, you will need [devkitPro](https://devkitpro.org/) with the devkitARM toolchain, plus the necessary tools and libraries. devkitPro includes `dkp-pacman` for easy installation of all components:

```
 $ dkp-pacman -Syu devkitARM general-tools dstools ndstool libnds libfat-nds
```

Once everything is downloaded and installed, `git clone` this repository, navigate to the folder in which it was cloned, and run `make` to compile the application. If there is an error, let us know.

## Credits
* RocketRobz: Creator of GodMode9i.
* FlameKat53: Leader of the rewrite :sunglasses:
* JeffRuLz: SD Card size/free getting code
* Jimmy-Z: term256, and an example of it's use in twlnf
* zacchi4k: Creator of the GodMode9i logo used in v1.3.1 and onwards.
* devkitPro/WinterMute: devkitARM, libnds, original nds-hb-menu code, and screenshot code.
* d0k3: Original GM9 app and name for the Nintendo 3DS, which this application is inspired by.
