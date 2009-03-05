Fire v2.1 
02-02-2009


Description: 
------------------
Fire is a lightweight themable GUI engine for j2me MIDP2 applications. It is designed to be an eye-candy replacement to the traditional midp GUI components. It provides much more functionality than the midp standard components and its not kvm-implementation depended like the midp standard components. 

Since v2.0 Fire also has an xHTML module that allows the creation of mobile browsers and GUIs based on xHTML. Fire also comes with a set of utility classes for logging, internationalization and more. 

License: 
--------------- 
Fire-j2me is licensed under the LGPL. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details. 


History: 
----------------- 
Fire initially started as a toy project to see if I could avoid some of the kvm-depended behavior of midp GUI components. Anyone who has ever tried to develop using the standard GUI components will soon discover that its almost impossible to develop an application that looks and runs correctly on more that a few phone models, even models of the same manufacturer. 

The toy project evolved into the Fire library which was used extensively by me and my company on tens of j2me projects over the past 2 years. Currently it is used as the GUI engine of the Bluevibe Browser (http://www.bluevibe.net). Apart from rendering the UI of the application and handling the animations,  the xHTML component of the library is used by the Bluevibe Browser to render web pages delivered over Bluetooth or WiFi connections to the cell phones of all Bluevibe users.


Changelog since v2.0: 
----------------------- 

Many additions and bug fixes where made since v2.0. The main additions include better javadoc and more code samples. 

- New PageListener Interface.  Can be set to receive loadPageCompleted events. Also displayPage is now called loadPageAsync 
- animationsEvabled flag in the FireScreen disables the build-in animations like scroll when needed by the 
  developer (i.e when running on a low-end phone). 
- Alerts have been improved to be easier to use and customize. 
- Better scrolling and navigation inside panels. The scrolling logic of the Panel is dramatically improved, 
  increasing the navigation speed and improving the end-user experience. 
- Improved xHTML component. Numerous bug fixes on the Browser now allow access to many more sites. 
- Improved i18n. Bug fixes on the HttpClient and other utility classes improve language support 
- Common layout. Now the Panel and the Container class have the same layout schematic 
- GridLayout now supports both vertical and horizontal navigation. 
- many more minor fixes 


Usage: 
--------------- 

You can use the library by adding the compiled classes or source to your project. Check the code 
samples in the Fire2Samples project and the javadoc for more information on using fire. 

Please use the project's home page on sourceforge.net to post questions (on the forum) and report bugs (on the bug tracker tool). 


Demo: 
-------------- 

In the Fire2Samples.jar there are sample midlets that demonstrate the capabilities of the library. You can use a j2me emulator like the like SUN's WTK or microemulator or install it on a mobile phone that supports midp2 cldc1.0 midlets. 

Package Contents:
--------------------------

README.txt: This file
doc: Fire and code samples javadoc
src: source of Fire2 and of code samples
classes: compiled fire2 class files
demo: Demonstration midlet, from the samples project (both obfuscated and normal version)



Acknowledgments: 
------------------ 

Thanks to everyone who contributed patches and submitted bugs, for this release. 
Special thanks to George Saslis and George Peponakis from bluevibe.net for their great support, bug fixes and recommendations, also to Frank for his bug fixes and contributions and to Maxim Blagov for the motorola fix. 


I hope that you will find this project usefull. 

You can send feedback, bug reports etc using the project's homepage on sourceforge (preferred): 

http://sourceforge.net/projects/fire-j2me 

 - or - 

contact me: padeler at bluevibe dot net 



