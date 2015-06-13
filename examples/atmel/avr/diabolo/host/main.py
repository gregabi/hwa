#!/usr/bin/env python
# -*- coding: utf-8 -*- Last modified: 2015-06-07 11:00:55 -*-


import sys
import os.path

from com import Com
from utils import s2hex, hexdump
import time

from crc_ccitt import CRC
from cscont import CsCont
from csmod import CsMod
from parse import get_args
import bootloader


#  Compute application CRC and length of application code
#
#    The CRC is computed from the end to the beginning of the memory, skipping
#    the 0xFF bytes at the end.
#
#    For devices without boot section, 'end' should be 1 so that the first two
#    bytes of reset vector (rjmp to Diabolo) are not computed.
#
def appstat ( data, bootsection ):
    # trace("bootsection = " + str(bootsection) )
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


class Application:
    def __init__(self, options):
        self.options = options
        self.diabolo = None
        self.mcu = None


    #  Read a binary file
    #
    def read_file(self, filename):
        if not os.path.isfile(filename):
            die(_("File %s does not exist.\n" % filename))
        if not filename.endswith('.bin'):
            die(_("File format not handled."))
        f = open(filename, 'rb')
        data = f.read()
        f.close()
        if not self.options.quiet and len(data)==0:
            cout(_("%s is void.\n" % filename))
        else:
            cout(_("Loaded %d bytes from %s.\n" % (len(data), filename)))
        return data


    #  Write a binary file
    #
    def write_file(self, filename, data):
        f = open(filename, 'wb')
        f.write(data)
        f.close()
        cout(_("Stored %d bytes into %s.\n" % (len(data), filename)))


    #  Get the diabolo object
    #
    def get_diabolo(self, options):
        #
        #  Open the communication interface
        #
        if options.carspy >= 0:
            options.bps = 230400
        com = Com(options.tty, options.bps, options.wires)
        cout(_("Interface: %s (%d bps)\n" % (options.tty, options.bps)))
        #
        #  Reach proper diabolo
        #
        if options.carspy==-1:
            cout(_("Standard device\n"))
            self.diabolo = bootloader.Diabolo(com)
            self.diabolo.reset_signal = options.reset
            self.diabolo.reset_length = options.reset_length
            return
        #
        #  Carspy controller or module
        #
        self.diabolo = CsCont(com)
        if not diabolo.connect():
            cout(_("  No carspy controller found. Already running Diabolo?\n\n"))
            self.diabolo = Diabolo(com)
            if options.carspy > 0:
                return
        else:
            cout(_("  Found Carspy controller: "))
            if not self.diabolo.identify():
                die(_("could not identify!\n"))
            cout(_("reference %04X, firmware %04X\n" % (self.diabolo.ref, self.diabolo.crc)))
            if options.carspy == 0:
                if not self.diabolo.diabolo():
                    die(_("    Could not launch Diabolo!\n"))
                cout(_("    Diabolo launched.\n\n"))
                self.diabolo = Diabolo(com)
            else:
                self.diabolo = CsMod(com, options.carspy)
                if not self.diabolo.connect():
                    cout(_("    Carspy module %d not found. Already running Diabolo?\n\n"
                           % options.carspy))
                    # return
                else:
                    cout(_("    Found Carspy module %d:" % options.carspy))
                    cout(_("reference %04X, firmware %04X\n" % (self.diabolo.ref, self.diabolo.crc)))
                    if not self.diabolo.diabolo():
                        die(_("      Could not launch Diabolo!\n"))
                    cout(_("      Diabolo launched.\n\n"))


    #  Read flash memory
    #
    def read_flash(self):
        t = time.time()
        cout("\nReading flash:")
        s=''
        vp = '\xFF'*self.device.pagesize
        col=0
        for a in range(0, self.device.flashsize, self.device.pagesize):
            if col==0 :
                cout('\n')
            col += 1
            if col==64:
                col=0
            if a < self.device.bladdr or self.options.full:
                p = self.diabolo.read_flash_page(a)
                if p == None:
                    cerr(_("\nRead page failed at 0x%04X.\n" % a))
                    return False
            if a < self.device.bladdr:
                if (a != 0 and p != vp) or (a == 0 and p[2:] != vp[2:]):
                    cout('X')
                else:
                    cout('.')
            else:
                if self.options.full:
                    cout('*')
                else:
                    cout('-')
                    p = vp		# Do not store Diabolo code in cache file
            flushout()
            s += p
        t = time.time()-t
        crc, x = appstat(s[:self.device.bladdr], self.device.bootsection)
        cout(_("\nRead %d bytes in %d ms (%d Bps): %d application bytes, CRC=0x%04X.\n"
               % (len(s), t*1000, len(s)/t, x, crc)))
        return s


    #  Write flash memory
    #
    def write_flash(self, data, cache):
        t = time.time()
        pg = 0
        col = 0
        redo = []
        errors = 0
        cout("\nProgramming flash:")
        for a in range(0, self.device.bladdr, self.device.pagesize):
            if col==0 :
                cout('\n')
            col += 1
            if col==64:
                col=0
            if cache \
               and data[a:a+self.device.pagesize] != cache[a:a+self.device.pagesize]:
                r = self.device.write_flash_page(a, data[a:a+self.device.pagesize])
                if r == 'T':
                    cout(_(" Timeout!\n"))
                    return
                if r == 'P':
                    redo.append(a)
                if r != '-':
                    pg += 1
                if r == 'L':
                    cerr(self.device.error+'\n')
            else:
                r = '-'
            cout(r)
            flushout()
        cout("\n")
        #
        #  Reprogram pages that failed
        #
        pv = '\xFF'*self.device.pagesize
        for a in redo:
            cout("\n")
            cout("Need to reprogram page at 0x%04X.\n" % a)
            p1 = data[a:a+self.device.pagesize]
            cout("To be programmed:\n%s\n" % hexdump(a, p1))
            p0 = self.diabolo.read_flash_page(a)
            if p0 != pv:
                r = self.diabolo.write_flash_page(a, pv)
                cout("Erasing: %s\n" % r )
                p0 = self.diabolo.read_flash_page(a)
                if p0 != pv:
                    cerr("Erasing failed.\n")
                    return
            if p0 != p1:
                r = self.diabolo.write_flash_page(a, p1)
                cout("Programming: %s\n" % r )
                p0 = self.diabolo.read_flash_page(a)
                if p0 != p1:
                    cerr(_("Contains:\n%s\n" % hexdump(a, p0)))
                    die("Programming failed.\n")
            cout("\n")

        #cout("\n")
        t = time.time()-t
        if pg:
            cout(_("Programmed %d pages (%d bytes) in %d ms (%d Bps).\n" %\
                   (pg, pg*self.device.pagesize, t*1000, pg*self.device.pagesize/t)))
        cout("\n")


    #  Read eeprom memory
    #
    def read_eeprom(self):
        t = time.time()
        cout("\nReading EEPROM:")
        s=''
        col=0
        for a in range(0, self.device.eepromsize, 64):
            if col==0 :
                cout('\n')
            col += 1
            if col==64:
                col=0
            p = self.diabolo.read_eeprom(a,64)
            if p == None:
                cerr(_("\nRead failed at 0x%04X.\n" % a))
                return False
            cout('.')
            flushout()
            s += p
        t = time.time()-t
        cout(_("\nRead %d bytes in %d ms (%d Bps).\n" % (len(s), t*1000, len(s)/t)))
        return s


    def run(self):
        flash = None			# Data to burn into flash

        start_time = time.time()

        #  Compute a CRC?
        #
        if self.options.stat or self.options.crc or self.options.size:
            if not self.options.filename:
                die(_("Input file is required for CRC computation.\n"))
            if not self.options.mcu:
                die(_("Target device is required for CRC computation.\n"))
            device = None
            for d in Device.dtbl:
                if d[0].lower() == self.options.mcu.lower():
                    device = d
            if not device:
                die(_("Unrecognized device name.\n"));

            data = self.read_file(self.options.filename)
            crc, x = appstat(data, device[5])
            if self.options.stat:
                sys.stdout.write(_("%s: %d /%d application bytes, CRC=0x%04X.\n" %
                                   (self.options.filename, x, len(data), crc)))
            elif self.options.crc:
                sys.stdout.write("%04X\n" % crc )
            else:
                sys.stdout.write("%d\n" % x )
            return

        #  Get the proper diabolo
        #
        self.get_diabolo(self.options)

        #  Just reset the device and quit?
        #
        if self.options.reset_and_exit:
            self.diabolo.reset()
            return

        #  Connect to the device
        #
        if not self.diabolo.connect():
            die(_("Could not connect.\n"))

        #  Get Diabolo prompt
        #
        if not self.diabolo.get_prompt():
            die(_("Could not get prompt: %s.\n" % self.diabolo.error))

        #  Identify device
        #
        while self.diabolo.com.rx(1): pass # flush
        self.device = self.diabolo.identify()
        if not self.device:
            die(_("Could not identify: %s.\n" % self.diabolo.error))
        cout(_("Connected to device (protocol %d):\n" % self.diabolo.protocol))
        cout(self.device.identify())

        if not self.diabolo.protocol in [2,3,4]:
            die(_("\nProtocol not supported\n"))

        #  Check target name
        #
        if self.options.mcu and self.device.name.lower() != self.options.mcu.lower():
            die(_("Target mismatch: connected to %s, %s expected.\n" %
                  (self.device.name, self.options.mcu)))

        #  Read flash memory?
        #
        if self.options.read_flash:
            data = self.read_flash()
            if self.options.hexdump:
                sys.stdout.write(hexdump(0,data)+"\n")
            if self.options.cache:
                self.write_file(self.options.cache, data)
            #return

        #  Read eeprom memory?
        #
        if self.options.read_eeprom:
            data = self.read_eeprom()
            if self.options.hexdump:
                sys.stdout.write(hexdump(0,data)+"\n")
            if self.options.cache:
                self.write_file(self.options.cache, data)
            #return

        #  Erase flash memory?
        #
        if self.options.clear_flash:
            flash = '\xFF'*self.device.flashsize

        #  Write flash memory?
        #
        if self.options.burn_flash:
            if not self.options.filename:
                die(_("Input file is required to program flash memory.\n"))
            cache = None
            if self.options.cache:
                if os.path.isfile(self.options.cache):
                    cache = self.read_file(self.options.cache)
                    cache_crc, x = appstat(cache[:self.device.bladdr],
                                                self.device.bootsection)
                    if self.diabolo.appcrc != cache_crc:
                        cout(_("Cache CRC does not match device CRC. Dropping cache data.\n"))
                        cache = None
                else:
                    cout(_("Cache file does not exist yet, reading flash"
                           " memory is required.\n"))
            if cache == None:
                cache = self.read_flash()
            cache_crc, x = appstat(cache[:self.device.bladdr],
                                        self.device.bootsection)
            flash = self.read_file(self.options.filename)

        #  Program flash memory
        #
        if flash:
            #
            #  Override RESET vector for device without bootsection
            #    RJMP opcode: 1111 aaaa aaaa aaaa = 0xC000 + Addr
            #
            if not self.device.bootsection:
                addr = self.device.bladdr/2 - 1
                if addr > 0x0FFF:
                    die(_("Can not set RESET vector opcode.\n"))
                opcode = 0xC000 + addr
                byte0 = opcode & 0xFF
                byte1 = opcode >> 8
                cout(_("Device without bootsection, "
                       "changed RESET vector opcode: %02x %02x\n")
                     % (byte0, byte1))
                flash = chr(byte0)+chr(byte1)+flash[2:]

            flash_crc, x = appstat(flash[:self.device.bladdr],
                                       self.device.bootsection)
            if flash_crc == cache_crc:
                cout(_("Cache and data CRC match. Nothing to do.\n"))
            else:
                self.write_flash(flash,cache)

            x_restart = False
            if self.device.flash_changed:
                self.diabolo.write_eeprom_pgmcount(self.device.pgmcount+1)
                cout(_("Set program count to %d\n" % self.device.pgmcount))
                x_restart = True

            if self.device.eeappcrc != flash_crc:
                self.diabolo.write_eeprom_appcrc(flash_crc)
                cout(_("Set stored application CRC to 0x%04X\n" % flash_crc))
                x_restart = True

            if x_restart:
                #cout(_("Reset device's Diabolo\n"))
                self.diabolo.tx('!') # Send a bad command to force CRC re-computation
                self.diabolo.get_prompt()
                self.diabolo.identify()
                cout("  Application CRC:\n")
                cout(_("    computed: 0x%04X\n" % self.device.appcrc))
                cout(_("      stored: 0x%04X\n" % self.device.eeappcrc))
                cout(_("  Programmings: %d\n" % self.device.pgmcount))

            #  Update flash cache
            #
            if self.options.cache:
                self.write_file(self.options.cache, flash)

        #  Start application
        #
        if self.device.appcrc == self.device.eeappcrc \
           and self.device.appcrc != 0xFFFF:
            #
            #  FIXME: with --read-flash --hexdump, device.run fails.
            #         device.identify makes it succeed. Why?
            #
            self.diabolo.identify()
            if self.diabolo.run():
                cout("Application started.\n")
            else:
                cout("Run failed.\n")

        self.diabolo.com.close()
        cout(_("%d commands, %d fails, %d resumes, %d resyncs, total time: %.1f s.\n" % 
               (self.diabolo.ncmds, self.diabolo.ncmdfails, self.diabolo.nresumes,
                self.diabolo.nresyncs, time.time()-start_time+0.05)))


if __name__ == "__main__":
    import premain
    import __builtin__
    try:
        options = get_args()
        if options.quiet:
            __builtin__.__dict__['cout'] = id
        #print options
        Application(options).run()
    except KeyboardInterrupt:
        cout("\n")
    # except Exception, e:
    #     cerr(str(e)+'\n')