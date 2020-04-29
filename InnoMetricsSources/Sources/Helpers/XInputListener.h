//
// Created by alon on 22.10.16.
//
#ifndef INNOMETRICS2_XINPUTLISTENER_H
#define INNOMETRICS2_XINPUTLISTENER_H

#include <iostream>
#include <cstring>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>

#define INVALID_EVENT_TYPE -1
#define XINAME        "XInputExtension"

class XInputListener {
private:
    static int xi_opcode;

public:
    static int MotionType;
    static int ButtonPressType;
    static int ButtonReleaseType;
    static int KeyPressType;
    static int KeyReleaseType;
    static int ProximityInType;
    static int ProximityOutType;

public:
    static int RegisterEvents(Display *dpy, XIDeviceInfo *info, Bool handle_proximity, Window window = -1);

    static XIDeviceInfo **GetDevices(Display *display, int &count);

    static int SelectXInputEvents(Display *display, Window window = -1);

private:
    XInputListener() {}

    // Determine version of extensions for X11
    static int CheckXInputVersion(Display *display);
};

#endif //INNOMETRICS2_XINPUTLISTENER_H
