# Emunandswitcher
SXOS and Atmosphere SD Files Emunand/Emummc Switcher and Multitool

![Downloads](https://img.shields.io/github/downloads/Storm21CH/Emunandswitcher/total)

Fork from: https://github.com/Guillem96/argon-nx

Vorstellung:

Nach einer Idee, habe ich begonnen diese Payload App zu entwickeln. Die App ist eher für Entwickler und Tester gedacht aber wer es brauchen kann, gerne...
Damit können gleichzeitig maximal drei Emunands auf der SD betrieben und mit ein paar Klicks gewechselt werden. 
Die Emunands und die auf SD installierten Inhalte sind alle von einander getrennt und können mit unterschiedlicher Systemsoftware Versionen upgedatet werden.

Das ganze funktioniert mit SXOS und Atmosphere SD Emunand/Emummc. Es funktioniert auf allen Systemsoftware Versionen (Payload), 
weil es eine RCM App ist und ausgeführt wird bevor irgend ein CFW oder Nintendo Code folgt.

![](screenshots/scr20210325_105032.jpg)

![](screenshots/scr20220405_162612.jpg)

![](screenshots/scr20220405_165448.jpg)

![](screenshots/scr20220407_003415.jpg)

![](screenshots/scr20220406_164513.jpg)

![](screenshots/scr20210321_161104.jpg)

![](screenshots/scr20210321_161128.jpg)

Release 1.1 Änderungen:

- Atmosphere SD emummc integriert
- Möglichkeit die gewünschte CFW beim start auszuwählen
- Neue Ordnerstruktur um besser zwischen SXOS und Atmosphere zu unterscheiden
- Akku Info by mrdude integriert
- LayeredFS und Theme tauglich, Titles Ordner integriert
- Theme Warnung und Möglichkeit Theme zu löschen integriert

Introduction:

After an idea, I started to develop this payload app. The app is rather for developers and testers thought but who can use it, with pleasure...
This can simultaneously operate a maximum of three Emunands on the SD card and be changed with a few clicks.
The Emunands and the content installed on SD are all separate and can be updated with different system software versions.

At the moment this is just for SXOS and Atmosphere SD Emunand/Emummc. It works on all system software versions (payload) because
it is an RCM app and will run before any CFW or Nintendo code follows.

Release 1.1 changes:

- Atmosphere SD emummc integrated
- Possibility to select the desired CFW at startup
- New folder structure to better distinguish between SXOS and Atmosphere
- Battery Info integrated by mrdude
- LayeredFS and Theme compatible, Titles folder integrated
- Theme warning and possibility to delete theme integrated

Release 2.0 Änderungen:

- Ganz neuer GUI, mit Live aktualisierung für Theme info, installierter Emunand Anzeige, Akku-, Datum- und Zeitanzeige.
- Zeit und Datum kommen von der RTC und kann falsch sein! Mit Ubuntu könnte die RTC gesetzt werden, funktioniert dann auch in Hekate richtig.
Fix GMT+1h und mit Sommerzeit Umschaltung.
- Emunand switcher für SXOS und Atmosphere mit den üblichen Funktionen, per Tabs umschaltbar.
- Möglichkeit zum Theme löschen vor dem booten für beide CFWs unter Tools.
- Update Möglichkeit für SXOS, boot.dat z.B. mit nxmtp auf die SD kopieren, dann im RCM Updaten, neu booten fertig
- Payload Loader Argon kompatibel, zeigt die Payloads aus dem argon Ordner an, zwei Reihen Payloads pro Tab

Release 2.0 changes:

- Completely new GUI, with live update for theme info, installed emunand display, battery, date and time display.
- Time and date come from the RTC and can be wrong! With Ubuntu the RTC could be set, works then also in Hekate correctly. Fix GMT + 1h and with summer time switching.
- Emunand switcher for SXOS and Atmosphere with the usual functions, switchable via tabs.
- Ability to delete theme before booting for both CFWs under Tools.
- Update possibility for SXOS, copy boot.dat e.g. with nxmtp on the SD, then in the RCM update, reboot
- Payload Loader Argon compatible, displays the payloads from the argon folder, two rows of payloads per tab

Release 2.1 Änderungen:

- Neuaufbau des Tools Tabs, da zusätzliche Funktionen
- CPU Temperatur Anzeige
- Stromverbrauch in mA und Akkuspannung in V, Anzeige bei laden grün, bei entladen rot
- Möglichkeit die Hellikeit des Displays per Slider einzustellen, die eingestellte Helligkeit wird gespeichert und bei neustart der App automatisch geladen.
- Bearbeiten der Config.ini direkt in der App, nach einem Softwareupdate eines Emunandes kann der Text direkt vor dem booten geändert werden.
- RTC Zeit und Datum setzen!!! Damit kann die Zeit der Realtimeclock eingestellt werden, damit das Datum im Emunandswitcher und hekate richtig angezeigt wird. 
Achtung: Die Zeit im Homemenü der Switch muss danach neu eingestellt werden. Es wird nachher im RCM und Switchmenü die selbe Zeit und Datum angezeigt!

New in Emunandswitcher 2.1:

- Reconstruction of the Tools tab because of additional functions
- CPU temperature display
- Current consumption in mA and battery voltage in V, display green when charging, red when discharging
- Possibility to adjust the brightness of the display using the slider, the set brightness is saved and automatically loaded when the app is restarted.
- Edit the Config.ini directly in the app, after a software update of an Emunand the text can be changed directly before booting.
- Set RTC time and date!!! The time of the real-time clock can be set so that the date is displayed correctly in the Emunandswitcher and hekate.
Attention: The time in the home menu of the switch must then be adjust. Afterwards the same time and date will be shown in the RCM and switch menu!

New Features 3.0:

- Build with newest hekate HW Libs
- Mariko Support
- Joycon Mouse
- UMS SD Card
- Hardware info Page
- Reload Menu
- Added Filemanager for Argon and Emunandswitcher
- Added Reboot Menu with Power Button
- Forwarder and Reboot to Payload files fixed for Atmosphere 19+
- Partition or SD Files Switcher*

Key assignment usage Filemanager:
- Touch screen as usual
- Joycon Mouse, A = Enter folder / Choose Buttons, B = Back Folder, X = Close Window
- Joypad up / down Filelist navigation, A = Enter folder, B = Back Folder, X = Close Window

*If SD files works (emummc.ini of SD files is in the folder "emummc"), the emummc.ini of a partition emummcs can be copied / renamed to "emunandswitcher/folders atm/emummc/emummc_part.ini". You can now switch between partition and SD files... The contents are temporarily in "emunandswitcher/folders atm/contentsPA" for partition or "emunandswitcher/folders atm/contentsSD" for SD files. 

Neue Funktionen 3.0:

- Build with newest hekate HW Libs
- Mariko Support
- Joycon Mouse
- UMS SD Card
- Hardware info Page
- Reload Menü Funktion
- Filemanager für Argon und Emunandswitcher hinzugefügt
- Reboot Menü mit Power Button hinzugefügt
- Forwarder und Reboot to Payload files fixed für Atmosphere 19+
- Partition oder SD Files Schalter*

Tastenbelegung Bedienung Filemanager:
- Touch screen wie gewohnt
- Joycon Mouse, A = Eingang Ordner / Button wählen, B = Ordner zurück, X = Fenster schliessen
- Joypad up / down Dateiliste navigation, A = Eingang Ordner, B = Ordner zurück, X = Fenster schliessen

*Wenn SD Files funktioniert (emummc.ini von SD Files ist in Ordner "emummc") kann die emummc.ini eines Partitions emummcs nach "emunandswitcher/folders atm/emummc/emummc_part.ini" kopiert / umbenannt werden. Es kann nun zwischen Partition und SD Files umgeschaltet werden... Die Contents sind jeweils in "emunandswitcher/folders atm/contentsPA" für Partition oder  "emunandswitcher/folders atm/contentsSD" für SD Files temporär.


New Features 3.1:

- Build with newest hekate HW Libs 5.5.7
- ATM Tab first
- Some Buttons Theme fix
- Fixed a Error after autoclosing a window with X Button (Close Filemanager and other Windows)

Neue Funktionen 3.1:

- Build with newest hekate HW Libs 5.5.7
- ATM Tab zuerst
- Bei ein paar Buttons Theme gefixt
- Ein Fehler wurde behoben nach dem autoschliessen eines Fensters mit dem X Button (Filemanager beenden und andere Fenster)


New Features 3.2:

- Typo fixed
- Tools Tab: Choose language switch added Deutsch / Englisch
- Selected language is saved in lang.ini GER / ENG
- Update zu v 3.2

Neue Funktionen 3.2:

- Typo gefixt
- Tools Tab: Sprachauswahl Schalter hinzugefügt Deutsch / Englisch
- Gewählte Sprache wird gespeichert in lang.ini GER / ENG
- Update zu v 3.2


New Features 3.3:

- Update to hekate bdk 5.7.2
- AULA OLED Switch support
- Hardware Info Page revised
- Payload ATM changed to fusee.bin, it is also possible with fusee-primary.bin but fusee.bin is preferred
- Fix slider brightness value for AULA
- Updated to 3.3
- General bug fixes and stability fixes

Thanks @dezem und https://gbatemp.net/members/twocool4life86.591345/ for the AULA OLED tests, without them this version would not have been possible!

Neue Funktionen 3.3:

- Update auf hekate bdk 5.72
- AULA OLED Switch support
- Hardware Info Page angepasst
- Payload ATM zu fusee.bin geändert, es ist auch mit fusee-primary.bin möglich aber fusee.bin ist bevorzugt
- Fix Slider Helligkeits Value für AULA
- Updated zu 3.3
- Allgemeine Fehlerbehebung und Stabilitäts Fixes

Danke an @dezem und https://gbatemp.net/members/twocool4life86.591345/ für die AULA OLED Tests, ohne sie wäre diese Version nicht möglich gewesen!


New Features 3.4:

- Menu colors can now be set arbitrarily:
- Textcolor
- Tab Textcolor
- Standard Payload Icon color

- Updated to 3.4
- General bug fixes and stability fixes

Neue Funktionen 3.4:

- Menü Farben können nun beliebig eingestellt werden:
- Textfarbe
- Tab Textfarbe
- Standart Payload Icon Farbe

- Update zu 3.4
- Allgemeine Fehlerbehebung und Stabilitäts Fixes

Thanks https://gbatemp.net/members/twocool4life86.591345/ for the color settings test.


Erstellt by Storm 2019 - 2022 mit Visual Studio, LittlevGL und DevKitPro, bassiert auf argonNX und hekate, Icons template von mrdude. Danke an die Programmierer! Danke auch an Gabor für LittlevGL!

Created by Storm 2019 - 2022 with Visual Studio, LittlevGL and DevKitPro, based on argonNX and hekate, Icons template and some code lines by @mrdude. Thanks to the programmers! Thanks also to Gabor for LittlevGL!

