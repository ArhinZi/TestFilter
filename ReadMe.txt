Test Filter 1.1
===============

This zip file contains the Test Filter executables and source code.

Executable files can be found in the "Executables" branch. 

Executables\Driver contains different subdirectories for different combinations of Windows version and processor architecture. Wlh is short for Windows Vista.

Executable\TestClt contains different subdirectories for different versions of Windows (5.01 = XP, 6.0 = Vista, 6.1 = Win7). Under these subdirectories, AMD64 contains the executable for x64 and x86 the one for the same-named architecture.

The source code is in the "Source" branch.

The documentation can be found into Source\Doc\TestFilterDriver.pdf. Refer to the "Source Code and Building" section on page 7 for details on the source code organization.

DESCRIPTION

This driver is a generic filter which can be applied on top of any other driver. It does not modify the IRPs it intercepts but passes them to the underlying filtered device. While doing this, it stores the IRPs along with other data into an internal buffer, which can be read by a client program. Thus, it makes possible to explore and log the IRPs targeted at the filtered device.

Together with each IRP, the content of the current I/O stack location is traced as well.

For IRP_MJ_READ and IRP_MJ_WRITE IRPs, the data is also traced. As an example, by installing this filter on top of a disk driver, it is possible to trace all the data buffers written to or read from a disk.

It should be easy to extend this filter by adding code specific to other major function codes, giving it the ability to trace the data of other kinds of IRPs besides read and write ones.

I developed this test filter driver mainly because I wanted to practice some Windows device driver programming, then I decided to publish it, together with its source code, because being able to trace IRPs targeted at a given device may be of some use and the code itself may be interesting to someone who, like me, is learning the ropes on this subject.

What's New in Version 1.1
This version includes a fix for a bug which caused a blue screen when the filter was used with certain kinds of devices (e. g. when filtering the serial port driver). Calls to DbgPrint in the driver code have been replaced with calls to DbgPrintEx, so that debug messages from the driver are printed even when the message filtering of Windows Vista and later versions is in effect. Builds for Windows 7 (both x64 and x86) are included and the client code can now format IRPs captured on Windows 7. See the Revision History section inside Source\Doc\TestFilterDriver.pdf

From: http://www.opening-windows.com/testfilter.html