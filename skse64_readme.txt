Skyrim Script Extender 64 v2.2.5 beta
by Ian Patterson and Stephen Abel (ianpatt and behippo)
Thanks to: Paul Connelly (scruggsywuggsy the ferret), gibbed, Purple Lunchbox, snakster
Special thanks to eternity for the help getting this finished. Can't thank you enough.

The Skyrim Script Extender 64, or SKSE64 for short, is a modder's resource that expands the scripting capabilities of Skyrim Special Edition. It does so without modifying the executable files on disk, so there are no permanent side effects.

Compatibility:

SKSE64 will support the latest version of Skyrim available on Steam and GOG. Please make sure you download the correct version for your store.
- Steam: 1.6.1130
- GOG: 1.6.659

Updates if needed will be available as soon as possible. Do not email to let us know that a new version has been released; we already know. Please be patient.

The Epic Store, Windows Store (gamepass), and console versions are not supported.

[ Installation ]

1. Copy the .dll and .exe files to your Skyrim SE directory. This is usually in your Program Files folder under Steam\SteamApps\common\Skyrim Special Edition\. If you see files named SkyrimSE and SkyrimSELauncher, this is the correct folder. Do not copy these files to the Data folder as with a normal mod. The "src" folder is only useful for programmers, most users can ignore it.

2. Copy the .pex files in Data\Scripts\ into the Data\Scripts\ folder of your installation. The .pex files are needed by all users of SKSE. 

3. If you create mods, copy the .psc files in Data\Scripts\Source\ into the Data\Scripts\Source\ folder of your installation. The .psc files are only needed if you have the CreationKit installed and intend to create or compile Papyrus scripts. Make sure to add them to your include path.

4. Run skse64_loader.exe to launch the game. You or your mod manager must continue to use the loader for SKSE64 to work.

[ Suggestions for Modders ]

If your mod requires SKSE64, please provide a link to the main SKSE website <https://skse.silverlock.org/> instead of packaging it with your mod install. Future versions of SKSE64 will be backwards compatibile, so including a potentially old version can cause confusion and/or break other mods which require newer versions.

[ Troubleshooting / FAQ ]

* Crashes or strange behavior:
 - Let us know how you made it crash, and we'll look into fixing it.

* XBone or PS4 version?
 - No. We do things that can't be done on consoles due to restrictions put in place by the manufacturers.

* My virus scanner complains about skse64_loader!
 - It is not a virus. To extend Skyrim and the editor, we use a technique called DLL injection to load our code. Since this technique can also be used by viruses, some badly-written virus scanners assume that any program doing it is a virus. Adding an exception to your scanner's rules may be necessary.

* I've followed the directions, but Skyrim SE still seems to launch without SKSE64!
- Try running skse64_loader.exe as an Administrator by right-clicking on skse64_loader.exe and selecting "Run As Administrator". This can be enabled as a compatibility option in the program's properties window. Note that this may run the game as a separate user, so your load order will need to be copied to the new user's profile.
 
* Can I modify and release my own version of SKSE based on the included source code?
 - No; the suggested method for extending SKSE is to write a plugin. If this does not meet your needs, please email the contact addresses listed below.

* How do I write Papyrus scripts using SKSE extensions?
 - If you've properly installed the .psc files from Data\Scripts\Source you can simply use the new functions listed.
 
* How do I know what SKSE functions have been added?
 - Look at the included .psc files in Data\Scripts\Source\. At the bottom of each .psc file is a label that shows the SKSE functions which have been added. Most have comments describing their purpose, if it is not obvious from the name.

* How do I write a plugin for SKSE?
 - See PluginAPI.h for instructions, as well as the example plugin project included with the rest of the source code. If you include any of the SKSE source code, you are required to make your source code available.

* Can I include SKSE as part of a mod pack/collection or otherwise rehost the files?
 - No. Providing a link to https://skse.silverlock.org/ is the suggested method of distribution. Exceptions may be given under applicable circumstances; contact us at the email addresses below. This means that if you see this file available for download anywhere other than https://skse.silverlock.org,  https://www.github.com/ianpatt/skse64, or on Nexus by any user other than https://www.nexusmods.com/users/2166624, that service is violating copyright. I don't like having to explicitly spell this out, but my hand has been forced.

* Do I need to keep old SKSE DLLs around for backwards compatibility?
 - No, they are only needed if you want to run old versions of the runtime with the last version of SKSE released for that version. Feel free to delete any skse_*.dll files that are not included with the main archive.

* Where did the log files go?
 - To support users on machines that don't have write access to the Program Files folder, they have been moved to the <My Documents>\My Games\Skyrim Special Edition\SKSE\ folder.

* Where is the skse.ini file?
 - SKSE does not include one by default. Create an empty text file in <skyrim root>\Data\SKSE\ named skse.ini. Create the SKSE folder if it doesn't already exist.

* How do I uninstall SKSE64?
 - Delete the .dll and .exe files starting with skse64_ from your Skyrim folder.

[ Contact the SKSE Team ]

Before contacting us, make sure that your game launches properly without SKSE64 first. If SKSE64 doesn't appear to be working, follow the steps in the FAQ first, then send us skse64.log and skse64_loader.log as attachments. These files may be found in <My Documents>\My Games\Skyrim Special Edition\SKSE\. Make sure you have "show file extensions" enabled.

### MAKE SURE TO INCLUDE YOUR LOG FILES AS ATTACHMENTS ###
We cannot help you solve load order problems. Do not email when a new version of the game is released.

Ian (ianpatt)
Send email to ianpatt+skse64 [at] gmail [dot] com

[ Standard Disclaimer ]

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
