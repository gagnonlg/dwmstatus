#!/bin/env python2

import datetime
import os
import time

import pytz as tz
import Xlib.display

class Battery(object):
    def __init__(self):

        base_path = '/sys/class/power_supply/BAT{}'
        i = 0
        while not os.path.isdir(base_path.format(i)):
            if i > 255:
                raise RuntimeError('Cannot find battery information')
            i += 1

        self.f_full = open(base_path.format(i) + '/energy_full', 'r')
        self.f_now = open(base_path.format(i) + '/energy_now', 'r')

    def __call__(self):

        self.f_full.seek(0)
        self.f_now.seek(0)

        full = float(self.f_full.read())
        now = float(self.f_now.read())

        nfull = int(10 * now / full)

        return '[' + nfull * '#' + (10 - nfull) * ' ' + ']'

class Time(object):
    def __init__(self, name, fmt, tzname):
        self.fmt = fmt
        self.tz = tz.timezone(tzname)
        self.name = name

    def __call__(self):
        t = datetime.datetime.now(self.tz)
        return '{}: {}'.format(self.name, t.strftime(self.fmt))


def main():

    components = [
        Battery(),
        Time('mtl', '%Y-%m-%d %H:%M', 'America/Montreal'),
        Time('gva', '%H:%M', 'Europe/Paris')
    ]

    rootwin = Xlib.display.Display().screen().root

    while True:
        rootwin.set_wm_name(' | '.join([fn() for fn in components]))
        print rootwin.get_wm_name()
        time.sleep(1)


if __name__ == '__main__':
    main()
