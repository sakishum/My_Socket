#!/usr/bin/env python
#coding=utf-8

import socket

def hack_fileobject_close():
    if getattr(socket._fileobject.close, '__hacked__', None):
        print 'return'
        return

    old_close = socket._fileobject.close

    def new_close(self, *p, **kw):
        try:
            return old_close(self, *p, **kw)
        except Exception, e:
            print "ignore %s." % str(e)
    new_close.__hacked__ = True
    socket._fileobject.close = new_close

if __name__ == '__main__':
    hack_fileobject_close()
    print 'Done.'
