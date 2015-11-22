#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

extern void init_shaders(void);
extern void draw(void);

static xcb_screen_t *
find_screen(xcb_connection_t *connection, int iscreen)
{
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(xcb_get_setup(connection));
    for (; it.rem; --iscreen, xcb_screen_next(&it)) {
        if (iscreen == 0)
            return it.data;
    }
    return NULL;
}

int
main(int argc, char *argv[])
{
    Display *legacy_display = XOpenDisplay(NULL);
    int iscreen = DefaultScreen(legacy_display);
    XSetEventQueueOwner(legacy_display, XCBOwnsEventQueue);
    xcb_connection_t *connection = XGetXCBConnection(legacy_display);

    do {

        int err = xcb_connection_has_error(connection);
        if (err) {
            fprintf(stderr, "Unable to open display.\n");
            break;
        }

        xcb_screen_t *screen = find_screen(connection, iscreen);
        if (! screen) {
            fprintf(stderr, "Screen %d not found.\n", iscreen);
            break;
        }

        xcb_window_t window = xcb_generate_id(connection);
        xcb_create_window(connection, XCB_COPY_FROM_PARENT,
                          window, screen->root,
                          0, 0, 640, 480, 0,
                          XCB_WINDOW_CLASS_INPUT_OUTPUT,
                          screen->root_visual,
                          0, 0);
        xcb_map_window(connection, window);

        xcb_flush(connection);

        bool done = false;

        EGLDisplay dpy = eglGetPlatformDisplay(EGL_PLATFORM_X11_KHR, (void *)legacy_display, NULL);

        if (! done) {
            bool ok = eglBindAPI(EGL_OPENGL_ES_API);
            if (! ok) {
                fprintf(stderr, "Can't bind OpenGL ES API\n");
                done = true;
            }
        }

        if (! done) {
            EGLint major = 0;
            EGLint minor = 0;
            EGLBoolean ok = eglInitialize(dpy, &major, &minor);
            if (! ok) {
                fprintf(stderr, "Error initializing EGL\n");
                done = true;
            } else {
                printf("Found EGL version %d.%d\n", major, minor);
            }
        }

        EGLSurface sfc = NULL;
        EGLContext ctx = NULL;
        EGLConfig cfg = NULL;

        if (! done) {
            const EGLint attrs[] = {
                EGL_SAMPLE_BUFFERS, 1,
                EGL_SAMPLES, 4,
                EGL_NONE
            };
            EGLConfig configs[1000];
            EGLint nconfigs = 0;
            EGLBoolean ok = eglChooseConfig(dpy, attrs, configs, 1000, &nconfigs);
            if (! ok) {
                fprintf(stderr, "Error choosing EGL config\n");
                done = true;
            } else {
                printf("Found %i EGL configs.\n", nconfigs);
            }
            if (nconfigs <= 0) {
                done = true;
            } else {
                cfg = configs[0];
            }
        }

        if (! done) {
            sfc = eglCreatePlatformWindowSurface(dpy, cfg, &window, NULL);
        }

        if (! done) {
            const EGLint attrs[] = { EGL_CONTEXT_CLIENT_VERSION, 3,  EGL_NONE };
            ctx = eglCreateContext(dpy, cfg, EGL_NO_CONTEXT, attrs);
        }

        if (! done) {
            EGLBoolean ok = eglMakeCurrent(dpy, sfc, sfc, ctx);
            if (! ok) {
                fprintf(stderr, "Can't bind EGL context\n");
                done = true;
            }
        }

        init_shaders();
        draw();
        eglSwapBuffers(dpy, sfc);

        xcb_generic_event_t *event = NULL;
        while (! done && (event = xcb_wait_for_event(connection))) {
            //...
            free(event);
        }

        eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        eglDestroyContext(dpy, ctx);
        eglDestroySurface(dpy, sfc);

        eglTerminate(dpy);
        xcb_destroy_window(connection, window);

    } while (0);
    XSetEventQueueOwner(legacy_display, XlibOwnsEventQueue);
    XCloseDisplay(legacy_display);

    return EXIT_SUCCESS;
}
