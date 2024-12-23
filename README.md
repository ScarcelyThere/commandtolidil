commandtolidil
==============

It's been months since I took this printer out, so I'm archiving this
repository. It achieved a level of functionality I'd describe as
"adequate," and I'm mildly proud of that.

What Is It?
-----------
Do you compile HPLIP with `--enable-hpcups-only` and miss knowing how
much ink you have left? Would you like to be able to print the
alignment page from your printer any time? Are you okay with level one
cleaning?

Then this is for you!

Beyond that, it's probably obvious it's a project where I try to
learn or remember C++. They tried to show me operator overloading and
templates, but I remember very little beyond classes. That's
changing, and thank you for coming along for the ride!

As another aside, I debated redoing this in C, as that's what CUPS is
written in. However, if you need this, you're using HPLIP, and the
part of that you'd be using is C++.

Notes on the Rename
-------------------
If you've been here since this was dj3600command, feel free to
uninstall the previous filter (check `/usr/lib/cups/filter/` for it,)
rebuild, and install.

You should also apply the `hp-deskjet_f4100.djtocommand.patch` patch
to change your PPD to use commandtolidil, or apply the supplied PPD
via CUPS' Web interface or something like `system-config-printer`.
At this time, only the Hewlett-Packard DeskJet F4180 All-in-One PPD
is included.

Obligatory Warning
------------------
This software is barely beta-quality. There's a test case, but it's
not comprehensive. My printer isn't often connected anymore. Upgrading
your printer should be considered a serious alternative to using this
software. Besides, who likes relocating one's laptop to within USB
cable range of one's printer?

Building It
-----------
`$ meson setup build`  
`$ meson compile -C build`  
Become root, then  
`# meson install -C build`  

You may want to restart CUPS. Re-add your DeskJet F4180 and select the
supplied PPD.

Should you be using my custom HPLIP Meson build, commandtolidil will
find the `hpmud` library and use that to query the printer if the
printer is set to use the HP backend. Using your distribution's HPLIP?
You probably don't need this project at all! Just issue `hp-levels`
from your favorite shell.

Be sure to use the supplied PPD file when setting up the printer. If
you've customized one, feel free to apply the supplied patch.

Known Supported Printers
------------------------
- Hewlett-Packard DeskJet F4180 All-in-One

Untested Printers
-----------------
- Every other printer ever

What About the Future?
----------------------
CUPS 3 is on the horizon, and it removes the entire filter system along with
PPD files. This renders commandtolidil useless. However, the
[pappl-retrofit](https://github.com/OpenPrinting/pappl-retrofit) 
project supplies `legacy-printer-app` which makes the existing CUPS 2.x
environment into an IPP Everywhere printer application. Since you're likely
using this from an existing CUPS 2.x installation, this should let you
continue using your DeskJet F4180 well into the future. I have not tested
this yet, but I hope to soon!

Using It
--------
This command filter implements the commands `Clean all`, `PrintSelfTestPage`,
`PrintAlignmentPage`, and `ReportLevels`, most of which you can issue from the
Web-based interface or a tool like `system-config-printer`.

`PrintSelfTestPage` prints the self-alignment scanning sheet one usually
gets when installing new ink cartridges and turning the printer on.
`PrintAlignmentPage` does the same thing.

`Clean all` performs a level one cleaning.

Examples
--------
My DeskJet F4180 is named "imagescrawler" in CUPS. Note it's using the
CUPS-supplied USB backend.
![An example Properties dialog](doc/properties.png)

Should a cartridge be absent, we only report on the installed cartridge.
![An example where only one cartridge is installed](doc/only_one_cartridge.png)

Here's a more usual result (right down to a nearly-empty Black cartridge.)
![An example where both cartridges are installed](doc/both_cartridges.png)

Acknowledgments
---------------
This would not have been possible without the open-source HPLIP drivers. My
appreciation goes to HP for making that software open and available. I'd also
like to thank the CUPS developers for excellent documentation and sample
code.

To Do
-----
- More test cases. We have tests for DeskJet status parsing and
cartridge data, but no other tests. Testing cleaning and alignment
page printing is being done on an actual DeskJet F4180. This is more
difficult to automate.
- It could be fun if we could determine whether or not the cartridge
was genuine.
- Somehow integrate this with hplip-printer-app, as that's the future
