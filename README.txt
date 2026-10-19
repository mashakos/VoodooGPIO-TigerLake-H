VoodooGPIO - Tiger Lake-H support (ACPI id INT34C6)
====================================================

WHAT THIS IS
Stock VoodooGPIO supports Tiger Lake-LP only (INTC1055). Tiger Lake-H laptops
(e.g. i7-11800H) have an INT34C6 GPIO controller with a different pin layout,
so VoodooI2C cannot use GPIO interrupts and falls over unless polling is forced.
This adds a VoodooGPIOTigerLakeH class. The pin tables, pad groups and
communities are ported from the Linux driver pinctrl-tigerlake.c (GPL-2.0).

STATUS: v2 - fixed start() to call VoodooGPIOIntel::start (v1 called VoodooGPIO::start, which skipped all setup). Needs hardware confirmation.
The table data was validated separately (291 pins, 5 communities, contiguous
pad groups, GPIO 44 -> hardware pin 37).

BEFORE YOU BUILD - sanity check in IORegistryExplorer
Search "GPI0". It must exist and its name/_HID must be INT34C6. If it is
missing or named PNP0C02, VoodooGPIO has nothing to attach to and this will
not help (GPI0 _STA / GPHD need fixing first).

NO XCODE? Build it on GitHub's free macOS machines instead - see
GITHUB-BUILD.txt in this folder (build.yml is the workflow).

BUILD WITH XCODE (needs Xcode + git; match the VoodooI2C version you run, 2.9.1)
  git clone --recursive https://github.com/VoodooI2C/VoodooI2C.git
  cd VoodooI2C
  git checkout v2.9.1        # NOTE: the tag has a leading "v" (commit e3ac3ed)
  git submodule update --init --recursive
  git clone https://github.com/acidanthera/MacKernelSDK
  cd Dependencies/VoodooGPIO
  git apply /path/to/tgl-h.patch
  xcodebuild -project VoodooGPIO.xcodeproj -target VoodooGPIO -configuration Release SYMROOT="$PWD/build"
The result is build/Release/VoodooGPIO.kext
(v2.9.1 pins VoodooGPIO commit 733dc80; the patch was checked against it.)

If "git apply" fails (the pinned VoodooGPIO differs from master):
  1. Copy the TigerLake-H folder into Dependencies/VoodooGPIO/VoodooGPIO/
  2. Open VoodooGPIO.xcodeproj in Xcode, right-click the VoodooGPIO group >
     "Add Files...", add both files in TigerLake-H (target: VoodooGPIO)
  3. Add this personality to VoodooGPIO/Info.plist under IOKitPersonalities:
       VoodooGPIOTigerLakeH
         CFBundleIdentifier  org.coolstar.VoodooGPIO
         IOClass             VoodooGPIOTigerLakeH
         IONameMatch         [ INT34C6 ]
         IOProviderClass     IOService
  4. Build as above.

INSTALL
Replace, in your EFI:
  EFI/OC/Kexts/VoodooI2C.kext/Contents/PlugIns/VoodooGPIO.kext
with the newly built VoodooGPIO.kext. No config.plist kext entry changes are
needed (same bundle path and executable path).

VERIFY after reboot (polling must be OFF - no -vi2c-force-polling boot arg and
no force-polling property on the I2C0 device)
  1. IORegistryExplorer: a VoodooGPIOTigerLakeH node under GPI0.
  2. ioreg -r -c VoodooI2CDeviceNub -l -w0 | head -5
     should show "registered, matched".
  3. A VoodooI2CHIDDevice under the touchpad nub.
