//
// Created by alon on 22.10.16.
//

#include "XInputListener.h"

int XInputListener::xi_opcode = 0;

int XInputListener::MotionType = INVALID_EVENT_TYPE;
int XInputListener::ButtonPressType = INVALID_EVENT_TYPE;
int XInputListener::ButtonReleaseType = INVALID_EVENT_TYPE;
int XInputListener::KeyPressType = INVALID_EVENT_TYPE;
int XInputListener::KeyReleaseType = INVALID_EVENT_TYPE;
int XInputListener::ProximityInType = INVALID_EVENT_TYPE;
int XInputListener::ProximityOutType = INVALID_EVENT_TYPE;

int XInputListener::RegisterEvents(Display *dpy, XDeviceInfo *info, Bool handle_proximity, Window window) {
    int number = 0;    /* number of events registered */
    XEventClass event_list[7];
    int i;
    XDevice *device;
    XInputClassInfo *ip;

    int screen = DefaultScreen(dpy);
    Window root_window = (window == -1)
            ? RootWindow(dpy, screen)
            : window;

    device = XOpenDevice(dpy, info->id);

    if (!device) {
        printf("Unable to open device '%lu'\n", info->id);
        return 0;
    }

    if (device->num_classes > 0) {
        for (ip = device->classes, i = 0; i < info->num_classes; ip++, i++) {
            switch (ip->input_class) {
                case KeyClass: {
                    DeviceKeyPress(device, KeyPressType, event_list[number]);
                    number++;
                    DeviceKeyRelease(device, KeyReleaseType, event_list[number]);
                    number++;
                    break;
                }
                case ButtonClass: {
                    DeviceButtonPress(device, ButtonPressType, event_list[number]);
                    number++;
                    DeviceButtonRelease(device, ButtonReleaseType, event_list[number]);
                    number++;
                    break;
                }
                case ValuatorClass: {
                    //HERE the code which handle mouse motions and stylus events.
                    /*
                     DeviceMotionNotify(device, MotionType, event_list[number]);
                     number++;
                     if (handle_proximity) {
                         ProximityIn(device, ProximityInType, event_list[number]);
                         number++;
                         ProximityOut(device, ProximityOutType, event_list[number]);
                         number++;
                     }
                     */
                    break;
                }
                default:
                    printf("unknown class\n");
                    break;
            }
        }

        if (XSelectExtensionEvent(dpy, root_window, event_list, number)) {
            printf("Error selecting extended events\n");
            return 0;
        }
    }
    return number;
}

XDeviceInfo **XInputListener::GetDevices(Display *display, int &count) {
    XDeviceInfo *devices;
    XDeviceInfo **found;
    int loop;
    int numOfDevices;
    XID id = (XID) -1;

    /*
        XI_MOUSE XI_TABLET XI_KEYBOARD XI_TOUCHSCREEN XI_TOUCHPAD
        XI_BUTTONBOX XI_BARCODE XI_TRACKBALL XI_QUADRATURE XI_ID_MODULE
        XI_ONE_KNOB XI_NINE_KNOB XI_KNOB_BOX XI_SPACEBALL XI_DATAGLOVE
        XI_EYETRACKER XI_CURSORKEYS XI_FOOTMOUSE XI_JOYSTICK
    */

    Atom xiMouse = XInternAtom(display, XI_MOUSE, false);
    Atom xiKeyBoard = XInternAtom(display, XI_KEYBOARD, false);
    Atom xiTouchPad = XInternAtom(display, XI_TOUCHPAD, false);


    devices = XListInputDevices(display, &numOfDevices);

    count = 0;
    for (loop = 0; loop < numOfDevices; loop++) {
        if ((devices[loop].use >= IsXExtensionDevice) &&
            ((devices[loop].type == xiMouse) ||
             (devices[loop].type == xiKeyBoard) ||
             (devices[loop].type == xiTouchPad)
            )) {
            count++;
        }
    }
    found = new XDeviceInfo *[count];
    int c = 0;
    for (loop = 0; loop < numOfDevices; loop++) {
        if ((devices[loop].use >= IsXExtensionDevice) &&
            ((devices[loop].type == xiMouse) ||
             (devices[loop].type == xiKeyBoard) ||
             (devices[loop].type == xiTouchPad)
            )) {
            found[c++] = &devices[loop];
        }
    }
    return found;
}

int XInputListener::CheckXInputVersion(Display *display) {
    XExtensionVersion *version;
    static int vers = -1;

    if (vers != -1)
        return vers;

    version = XGetExtensionVersion(display, INAME);

    if (version && (version != (XExtensionVersion *) NoSuchExtension)) {
        vers = version->major_version;
        XFree(version);
    }

    return vers;
}

int XInputListener::SelectXInputEvents(Display *display, Window window) {
    if (display == NULL)
        return -1;

    //Checking the XInput Library
    int event, error;
    if (!XQueryExtension(display, XINAME, &xi_opcode, &event, &error)) {
        printf("X Input extension not available.\n");
        return -1;
    }

    //Checking the version of library
    if (!CheckXInputVersion(display)) {
        printf("%s extension not available\n", INAME);
        return -1;
    }

    //TODO: Need to implement the function of getting the id automatically

    XDeviceInfo **infos;

    Bool handle_proximity = True;

    int count;
    infos = GetDevices(display, count);
    //printf("%d\n", count);
    for (int loop = 0; loop < count; loop++) {
        if (RegisterEvents(display, infos[loop], handle_proximity, window))
            continue;
        else {
            fprintf(stderr, "One of events did not register...\n");
            return -1;
        }
    }
    return 0;
}
