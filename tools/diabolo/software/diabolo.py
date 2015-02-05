#!/usr/bin/env python
# -*- coding: utf-8 -*- Last modified: 2015-02-05 19:23:27 -*-


#import struct
import sys

from com import Com
from utils import s2hex, hexdump
import time

from crc_ccitt import CRC
from device import Device
from cscont import CsCont
from csmod import CsMod
from parse import get_args


# def parse_options():
#     from optparse import OptionParser
#     parser = OptionParser(usage="usage: %prog [options] [firmware]")
#     parser.add_option("-d", "--dev", metavar="DEV", type="string",
#                       default="/dev/ttyUSB0", dest="comdev", 
#                       help="set com device (default /dev/ttyUSB0)")
#     parser.add_option("-b", "--bps", metavar="BPS", type="int",
#                       default=115200, dest="combps", 
#                       help="set com speed (default 115200)")
#     parser.add_option("-1", "--one-wire", action="store_true",
#                       default=True, dest="onewire",
#                       help="RX/TX on the same communication wire")
#     parser.add_option("-2", "--two-wires", action="store_false",
#                       default=True, dest="onewire",
#                       help="RX/TX on separate communication wires")
#     parser.add_option("-m", "--mcu", metavar="MCU", type="string",
#                       default=None, dest="mcu",
#                       help="target microcontroller")
#     parser.add_option("--read-flash", action="store_true",
#                       default=False, dest="read_flash",
#                       help="Force reading of flash memory")
#     parser.add_option("--full", action="store_true",
#                       default=False, dest="full",
#                       help="process full content (include bootloader code)")
#     parser.add_option("--hexdump", action="store_true",
#                       default=False, dest="hexdump",
#                       help="Hexdump read content")
#     parser.add_option("--flash-file", metavar="FILE", type="string",
#                       default=None, dest="flash_file",
#                       help="binary or hex file to burn into flash")
#     parser.add_option("--flash-cache", metavar="FILE", type="string",
#                       dest="flash_cache",
#                       help="binary file used to store flash content")
#     parser.add_option("-F", "--erase-flash", action="store_true",
#                       default=False, dest="erase_flash",
#                       help="Erase flash memory")
#     parser.add_option("--dump-eeprom", action="store_true",
#                       default=False, dest="dump_eeprom",
#                       help="Dump content of EEPROM memory")
#     # parser.add_option("-x", "--run", metavar="FILE", type="string",
#     #                   dest="run_cmd",
#     #                   help="command to start when the 'run' button is pressed")
#     parser.add_option("--carspy", metavar="MODULE", type="int", default=-1,
#                       dest="carspy",
#                       help="address (0..30) of Carspy module to access (0 for carspy controller)")
#     parser.add_option("-c", "--crc", action="store_true",
#                       default=False, dest="crc",
#                       help="compute firmware crc")
#     parser.add_option("-r", "--reset-only", action="store_true",
#                       default=False, dest="reset",
#                       help="reset the device and exit")
#     parser.add_option("--reset-pin", metavar="PIN", type="string",
#                       default="DTR", dest="reset_pin",
#                       help="RS232 signal used to reset the device")
#     parser.add_option("-t", "--reset-duration", type="float",
#                       default=0.01, dest="reset_duration",
#                       help="how long RTS is set low for reset (only for standard devices)")
#     # parser.add_option("-t", "--test", action="store_true",
#     #                   default=False, dest="test",
#     #                   help="test command")
#     (options, args) = parser.parse_args()
#     if len(args) == 1:
#         options.flash_file = args[0]
#     return options


#  Application CRC and length of application code
#
#    The CRC is computed from the end to the beginning of the memory, skipping
#    the 0xFF bytes at the end.
#
#    For devices without boot section, 'end' should be 1 so that the first two
#    bytes of reset vector (rjmp to Diabolo) are not computed.
#
def appstat ( data, bootsection ):
    crc = CRC.init()
    if not bootsection:
        end = 1
    else:
        end = -1
    for x in range(len(data)-1, end, -1):
        if data[x] != '\xFF':
            break
    for i in range(x, end, -1):
        crc = CRC.add(crc, data[i])
    return crc, x+1


#  Open the communication device
#    Return the com device if OK, None otherwise
#
# def open_com(options):
#     # trace()

#     # import os, stat
#     # if stat.S_ISFIFO(os.stat(self.options.comdev).st_mode):
#     #     cout( "File: %s is a FIFO\n" % self.options.comdev )
#     # elif stat.S_ISSOCK(os.stat(self.options.comdev).st_mode):
#     #     cout( "File: %s is a SOCKET\n" % self.options.comdev )

#     com = Com( options.comdev, options.combps )
#     # if com == None:
#     #     cerr(_("Can not get serial port\n"))
#     #     return None

#     try:
#         com.open()
#     except ValueError:
#         cerr(_("Value error\n"))
#     if not com.is_open():
#         cerr(_("Can not open serial port\n"))
#         return None

#     com.onewire = options.onewire
#     #com.set_timeout( max( 0.01, 20.0/options.combps) )
#     #com.setTimeout( 40.0/options.combps )
#     return com


def run():
    start_time = time.time()
    import os.path

    toburn = None
    #options = parse_options()
    options = get_args()

    print options

    #  Check that the target name is provided
    #
    # if not options.mcu:
    #     cerr(_("Device name must be provided with -m or --mcu.\n"))
    #     return

    #  Load file to burn into flash if any
    #
    if options.filename:
        # if not options.mcu:
        #     cerr("Target device must be provided.\n");
        #     return

        # device = None
        # for d in Device.dtbl:
        #     if d[0].lower() == options.mcu.lower():
        #         device = d
        #         break
        # if not device:
        #     cerr("Unrecognized device name.\n");
            # return

        if not os.path.isfile(options.filename):
            cerr(_("File %s does not exist.\n" % options.filename))
            return
        if not options.filename.endswith('.bin'):
            cerr(_("File format not handled."))
            return

        f = open(options.filename, 'rb')
        toburn = f.read()
        f.close()

        if len(toburn)==0:
            cout(_("%s is void.\n" % options.filename))
            return

        #  Overwrite the RESET vector of devices that do not have a
        #  bootloader section with a rjmp to Diabolo
        #    BFCE
        #
        # if options.mcu.lower == 'attiny84':
        #toburn = byte0 + byte1 + toburn[2:]

        # toburn_crc, x = appstat(toburn, device[5])
        # cout(_("%s: %d /%d application bytes, CRC=0x%04X.\n" %
        #        (options.filename, x, len(toburn), toburn_crc)))

    #  Compute a CRC?
    #
    if options.crc:
        if not toburn:
            cerr(_("Input file is required for CRC computation.\n"))
            sys.exit(1)
        if not options.mcu:
            cerr(_("Target device is required for CRC computation.\n"))
            sys.exit(1)
        for d in Device.dtbl:
            if d[0].lower() == options.mcu.lower():
                crc, x = appstat(toburn, d[5])
                cout(_("%s: %d /%d application bytes, CRC=0x%04X.\n" %
                       (options.filename, x, len(toburn), crc)))
                return
        cerr("Unrecognized device name.\n");
        sys.exit(1)

    #  Check that file and device match
    #
    if options.filename and options.device:
        toburn_crc, x = appstat(toburn, device[5])
        cout(_("%s: %d /%d application bytes, CRC=0x%04X.\n" %
               (options.filename, x, len(toburn), toburn_crc)))

    #  Open the communication interface
    #
    if options.carspy >= 0:
        options.bps = 230400
    com = Com(options.tty, options.bps, options.wires)
    cout(_("Interface: %s (%d bps)\n" % (options.tty, options.bps)))

    #  Reach proper device
    #
    if options.carspy==-1:
        cout(_("Standard device\n"))
        device = Device(com)
        device.reset_signal = options.reset
        device.reset_length = options.reset_length

        #  Detect how many wires are used for rx/tx
        #    This must be done with the device in reset state
        #
        # if com.wires == 0:
        #     cout(_("Tty wires:"))
        #     device.set_reset(True)
        #     time.sleep(0.1)
        #     com.serial.write('?')
        #     r = com.serial.read(10)
        #     device.set_reset(False)
        #     trace(s2hex(r))
        #     if r==None or r=='' or r=='!':
        #         wires = 2
        #     elif r=='?' or r == '?!':
        #         wires = 1
        #     else:
        #         raise Exception("Can not handle reply: \"%s\" %s." % (r,s2hex(r)))
        #     com.wires = wires
        #     cout(" %d\n" % com.wires)

        if options.reset_and_quit:
            device.reset()
            return

        if not device.connect():
            cerr(_("Could not connect.\n"))
            return

        cout(_("Connected to device \n"))

    else:
        #
        #  Carspy controller or module
        #
        device = CsCont(com)
        if not device.connect():
            cout(_("  No carspy controller found. Already running Diabolo?\n\n"))
            device = Device(com)
            if options.carspy > 0:
                return
        else:
            cout(_("  Found Carspy controller: "))
            if not device.identify():
                cout(_("could not identify!\n"))
                return
            cout(_("reference %04X, firmware %04X\n" % (device.ref, device.crc)))
            if options.carspy == 0:
                if not device.diabolo():
                    cout(_("    Could not launch Diabolo!\n"))
                    return
                cout(_("    Diabolo launched.\n\n"))
                device = Device(com)
            else:
                device = CsMod(com, options.carspy)
                if not device.connect():
                    cout(_("    Carspy module %d not found. Already running Diabolo?\n\n"
                           % options.carspy))
                    # return
                else:
                    cout(_("    Found Carspy module %d:" % options.carspy))
                    cout(_("reference %04X, firmware %04X\n" % (device.ref, device.crc)))
                    if not device.diabolo():
                        cout(_("      Could not launch Diabolo!\n"))
                        return
                    cout(_("      Diabolo launched.\n\n"))

    #  Check that file and device match
    #
    # if options.filename and options.device:
    #     toburn_crc, x = appstat(toburn, device[5])
    #     cout(_("%s: %d /%d application bytes, CRC=0x%04X.\n" %
    #            (options.filename, x, len(toburn), toburn_crc)))

    #  Get Diabolo prompt
    #
    if not device.get_prompt():
        cerr(_("Could not get prompt: %s.\n" % device.error))
        return

    #  Identify device
    #
    if not device.identify():
        cerr(_("Could not identify: %s.\n" % device.error))
        return

    #  Display device informations
    #
    # display(device)
    cout("Device identification:\n")
    cout(_("  Protocol: %d\n" % device.protocol))
    cout(_("  Signature: %s \"%s\"\n" % (device.signature, device.name)))
    cout(_("  Bootloader: 0x%04X (%d bytes for application)\n" % (device.bladdr, device.bladdr)))
    cout(_("  Fuses ext byte: 0x%02X\n" % ord(device.fuses[2])))
    cout(_("  Fuses high byte: 0x%02X\n" % ord(device.fuses[3])))
    cout(_("  Fuses low byte: 0x%02X\n" % ord(device.fuses[0])))
    cout(_("  Fuses lock bits: 0x%02X\n" % ord(device.fuses[1])))

    cout("  Application CRC:\n")
    cout(_("    computed: 0x%04X\n" % device.appcrc))
    cout(_("      stored: 0x%04X\n" % device.eeappcrc))
    if device.protocol < 3:
        cout(_("      status: %02X\n" % device.appstat))
    cout(_("  Programmings: %d\n" % device.pgmcount))

    if not device.protocol in [2,3,4]:
        cout(_("\nProtocol not supported\n"))
        return

    #  Check target
    #
    if options.mcu:
        if device.name.lower() != options.mcu.lower():
            cerr(_("Target mismatch: connected to %s, %s expected.\n" %
                   (device.name, options.mcu)))
            return

    #  Override RESET vector for device without bootsection
    #    RJMP opcode: 1111 aaaa aaaa aaaa = 0xC000 + Addr
    #
    if toburn and not device.bootsection:
        addr = device.bladdr/2 - 1
        if addr > 0x0FFF:
            cerr(_("Can not set RESET vector opcode.\n"))
            return
        opcode = 0xC000 + addr
        byte0 = opcode & 0xFF
        byte1 = opcode >> 8
        cout(_("Device without bootsection, RESET vector opcode: %02x %02x\n")
             % (byte0, byte1))
        toburn = chr(byte0)+chr(byte1)+toburn[2:]
        #cout(hexdump(0, toburn)+'\n')
        #return

    #  Load cache for flash data if wanted
    #
    cache = None
    if options.cachename:
        if os.path.isfile(options.cachename):
            if not options.cachename.endswith('.bin'):
                cerr(_("File format not handled."))
                return
            f = open(options.cachename, 'rb')
            cache = f.read()
            f.close()

            cache_crc, x = appstat(cache, device.bootsection)
            cout(_("%s: %d /%d application bytes, CRC=0x%04X.\n" %
                   (options.cachename, x, len(cache), cache_crc)))
            if device.appcrc != cache_crc:
                cerr(_("flash-cache and application CRC do not match!"))
                options.read_flash = True
        else:
            options.read_flash = True

    #  Dump content of EEPROM
    #
    if options.read_eeprom:
        t = time.time()
        cout("\nReading EEPROM:")
        s=''
        col=0
        for a in range(0, device.eepromsize, 64):
            if col==0 :
                cout('\n')
            col += 1
            if col==64:
                col=0
            p = device.read_eeprom(a,64)
            if p == None:
                cerr(_("\nRead failed at 0x%04X.\n" % a))
                return False
            cout('.')
            flushout()
            s += p
        t = time.time()-t
        cout(_("\nRead %d bytes in %d ms (%d Bps).\n" % (len(s), t*1000, len(s)/t)))
        if (options.hexdump):
            cout(hexdump(0,s)+"\n")


    #  Read flash memory if needed
    #
    if options.read_flash:
        t = time.time()
        cout("\nReading flash:")
        s=''
        vp = '\xFF'*device.pagesize
        col=0
        if options.full:
        # if options.hexdump_fullflash:
            end=device.flashsize
        else:
            end=device.bladdr
        for a in range(0, end, device.pagesize):
            if col==0 :
                cout('\n')
            col += 1
            if col==64:
                col=0
            p = device.read_flash_page(a)
            if p == None:
                cerr(_("\nRead page failed at 0x%04X.\n" % a))
                return False
            if a < device.bladdr:
                if (a != 0 and p != vp) or (a == 0 and p[2:] != vp[2:]):
                    cout('X')
                else:
                    cout('.')
            else:
                cout('-')
                if not options.full:
                    p = vp		# Do not store Diabolo code in cache file
            flushout()
            s += p
        device.flash = s
        t = time.time()-t
        crc, x = appstat(device.flash[:device.bladdr],device.bootsection)
        cout(_("\nRead %d bytes in %d ms (%d Bps): %d application bytes, CRC=0x%04X.\n"
               % (len(device.flash), t*1000, len(device.flash)/t, x, crc)))

        if options.hexdump:
            cout(hexdump(0,device.flash)+"\n")

        if crc != device.appcrc:
            cout(_("WARNING: Application CRC mismatch!\n"))
            #return

        if options.cachename:
            cache = device.flash
    else:
        device.flash = cache

    #  Erase flash
    #
    if options.clear_flash:
        if toburn:
            cerr(_("Can not erase flash when data are to be burned!\n"))
            return
        toburn = '\xFF'*device.flashsize
        toburn_crc = 0xFFFF

    #  Perform programming
    #
    if toburn:
        t = time.time()
        pg = 0
        col = 0
        redo = []
        errors = 0
        cout("\nProgramming flash:")
        for a in range(0, device.bladdr, device.pagesize):
            if col==0 :
                cout('\n')
            col += 1
            if col==64:
                col=0
            r = device.write_flash_page(a, toburn[a:a+device.pagesize])
            if r == 'T':
                cout(_(" Timeout!\n"))
                return
            if r == 'P':
                redo.append(a)
            if r != '-':
                pg += 1
            if r == 'L':
                cerr(device.error+'\n')
            cout(r)
            flushout()
        cout("\n")
        #
        #  Reprogram pages that failed
        #
        pv = '\xFF'*device.pagesize
        for a in redo:
            cout("\n")
            cout("Need to reprogram page at 0x%04X.\n" % a)
            p1 = toburn[a:a+device.pagesize]
            cout("To be programmed:\n%s\n" % hexdump(a, p1))
            p0 = device.read_flash_page(a)
            if p0 != pv:
                r = device.write_flash_page(a, pv)
                cout("Erasing: %s\n" % r )
                p0 = device.read_flash_page(a)
                if p0 != pv:
                    cerr("Erasing failed.\n")
                    return
            if p0 != p1:
                r = device.write_flash_page(a, p1)
                cout("Programming: %s\n" % r )
                p0 = device.read_flash_page(a)
                if p0 != p1:
                    cout("Contains:\n%s\n" % hexdump(a, p0))
                    cerr("Programming failed.\n")
                    return
            cout("\n")

        #cout("\n")
        t = time.time()-t
        if pg:
            cout(_("Programmed %d pages (%d bytes) in %d ms (%d Bps).\n" %\
                   (pg, pg*device.pagesize, t*1000, pg*device.pagesize/t)))
        cout("\n")

        x_restart = False
        if device.flash_changed:
            #trace()
            device.write_eeprom_pgmcount(device.pgmcount+1)
            cout(_("Set program count to %d\n" % device.pgmcount))
            #crc, x = appstat(toburn)
            x_restart = True

        if device.eeappcrc != toburn_crc:
        #if device.flash_changed or device.eeappcrc != crc:
            #trace()
            device.write_eeprom_appcrc(toburn_crc)
            cout(_("Set stored application CRC to 0x%04X\n" % toburn_crc))
            x_restart = True

        if x_restart:
            cout(_("Reset device's Diabolo\n"))
            device.tx('!') # Send a bad command to force CRC re-computation
            device.get_prompt()
            device.identify()
            cout("  Application CRC:\n")
            cout(_("    computed: 0x%04X\n" % device.appcrc))
            cout(_("      stored: 0x%04X\n" % device.eeappcrc))
            cout(_("  Programmings: %d\n" % device.pgmcount))

        if options.cachename and cache:
            cache = toburn

    #  Update cache
    #
    if options.cachename and cache:
        f = open(options.cachename, 'wb')
        f.write(cache)
        f.close()
        cout(_("Stored %d bytes in %s." % (len(cache), options.cachename)))

    #  Test command
    #
    # if options.test:
    #     data = ""
    #     for i in range(device.pagesize):
    #         data+=chr(i)
    #     # cout("Writing:\n")
    #     # r = device.write_flash_page(0x800, data);
    #     # cout("Result: %s\n" % r)

    #     cout("Writing eeprom:\n")
    #     device.write_eeprom(0, data[:4]);

    #  Start application
    #
    if device.appcrc == device.eeappcrc and \
            device.appcrc != 0xFFFF:
        if device.run():
            cout("Application started.\n")
        else:
            cout("Run failed.\n")

    com.close()
    cout(_("%d commands, %d fails, %d resumes, %d resyncs, total time: %.1f s.\n" % 
           (device.ncmds, device.ncmdfails, device.nresumes, device.nresyncs,
            time.time()-start_time+0.05)))


if __name__ == "__main__":
    # from premain import *
    import premain
    try:
        run()
    except KeyboardInterrupt:
        cout("\n")
