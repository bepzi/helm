/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2020 - Raw Material Software Limited

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 6 End-User License
   Agreement and JUCE Privacy Policy (both effective as of the 16th June 2020).

   End User License Agreement: www.juce.com/juce-6-licence
   Privacy Policy: www.juce.com/juce-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

// FIXME: This hack is quite kludgy and shouldn't be necessary. But it's VERY useful.
// #define NASTY_OPENGL_DEBUG_HACK

#ifdef NASTY_OPENGL_DEBUG_HACK
static void APIENTRY debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    // Some debug messages are just annoying informational messages
    switch (id)
    {
    case 131185: // glBufferData
        return;
    }

    printf("Message: %s\n", message);
    printf("Source: ");

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        printf("API");
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        printf("Window System");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        printf("Shader Compiler");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        printf("Third Party");
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        printf("Application");
        break;
    case GL_DEBUG_SOURCE_OTHER:
        printf("Other");
        break;
    }

    printf("\n");
    printf("Type: ");

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        printf("Error");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        printf("Deprecated Behavior");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        printf("Undefined Behavior");
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        printf("Portability");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        printf("Performance");
        break;
    case GL_DEBUG_TYPE_MARKER:
        printf("Marker");
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        printf("Push Group");
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        printf("Pop Group");
        break;
    case GL_DEBUG_TYPE_OTHER:
        printf("Other");
        break;
    }

    printf("\n");
    printf("ID: %d\n", id);
    printf("Severity: ");

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        printf("High");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        printf("Medium");
        break;
    case GL_DEBUG_SEVERITY_LOW:
        printf("Low");
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        printf("Notification");
        break;
    }

    printf("\n\n");
}
#endif

namespace juce
{

extern XContext windowHandleXContext;

//==============================================================================
// Defined juce_linux_Windowing.cpp
void juce_LinuxAddRepaintListener (ComponentPeer*, Component* dummy);
void juce_LinuxRemoveRepaintListener (ComponentPeer*, Component* dummy);

//==============================================================================
class OpenGLContext::NativeContext
{
private:
    struct DummyComponent  : public Component
    {
        DummyComponent (OpenGLContext::NativeContext& nativeParentContext)
            : native (nativeParentContext)
        {
        }

        void handleCommandMessage (int commandId) override
        {
            if (commandId == 0)
                native.triggerRepaint();
        }

        OpenGLContext::NativeContext& native;
    };

public:
    NativeContext (Component& comp,
                   const OpenGLPixelFormat& cPixelFormat,
                   void* shareContext,
                   bool /*useMultisampling*/,
                   OpenGLVersion)
        : component (comp), contextToShareWith (shareContext), dummy (*this)
    {
        display = XWindowSystem::getInstance()->getDisplay();

        XWindowSystemUtilities::ScopedXLock xLock;

        X11Symbols::getInstance()->xSync (display, False);

        GLint attribs[] =
        {
            GLX_RENDER_TYPE,      GLX_RGBA_BIT,
            GLX_DOUBLEBUFFER,     True,
            GLX_RED_SIZE,         cPixelFormat.redBits,
            GLX_GREEN_SIZE,       cPixelFormat.greenBits,
            GLX_BLUE_SIZE,        cPixelFormat.blueBits,
            GLX_ALPHA_SIZE,       cPixelFormat.alphaBits,
            GLX_DEPTH_SIZE,       cPixelFormat.depthBufferBits,
            GLX_STENCIL_SIZE,     cPixelFormat.stencilBufferBits,
            GLX_ACCUM_RED_SIZE,   cPixelFormat.accumulationBufferRedBits,
            GLX_ACCUM_GREEN_SIZE, cPixelFormat.accumulationBufferGreenBits,
            GLX_ACCUM_BLUE_SIZE,  cPixelFormat.accumulationBufferBlueBits,
            GLX_ACCUM_ALPHA_SIZE, cPixelFormat.accumulationBufferAlphaBits,
            GLX_X_RENDERABLE,     True,
            None
        };

        int countFbConfigs;
        fbConfig = glXChooseFBConfig (display, DefaultScreen (display), attribs, &countFbConfigs);
        if (fbConfig == nullptr)
            return;

        bestVisual = glXGetVisualFromFBConfig (display, *fbConfig);
        if (bestVisual == nullptr) {
            X11Symbols::getInstance()->xFree (fbConfig);
            return;
        }

        auto* peer = component.getPeer();
        jassert (peer != nullptr);

        auto windowH = (Window) peer->getNativeHandle();
        auto colourMap = X11Symbols::getInstance()->xCreateColormap (display, windowH, bestVisual->visual, AllocNone);

        XSetWindowAttributes swa;
        swa.colormap = colourMap;
        swa.border_pixel = 0;
        swa.event_mask = embeddedWindowEventMask;

        auto glBounds = component.getTopLevelComponent()
                           ->getLocalArea (&component, component.getLocalBounds());

        if (JUCEApplicationBase::isStandaloneApp())
            glBounds = Desktop::getInstance().getDisplays().logicalToPhysical (glBounds);

        embeddedWindow = X11Symbols::getInstance()->xCreateWindow (display, windowH,
                                                                   glBounds.getX(), glBounds.getY(),
                                                                   (unsigned int) jmax (1, glBounds.getWidth()),
                                                                   (unsigned int) jmax (1, glBounds.getHeight()),
                                                                   0, bestVisual->depth,
                                                                   InputOutput,
                                                                   bestVisual->visual,
                                                                   CWBorderPixel | CWColormap | CWEventMask,
                                                                   &swa);

        X11Symbols::getInstance()->xSaveContext (display, (XID) embeddedWindow, windowHandleXContext, (XPointer) peer);

        X11Symbols::getInstance()->xMapWindow (display, embeddedWindow);
        X11Symbols::getInstance()->xFreeColormap (display, colourMap);

        X11Symbols::getInstance()->xSync (display, False);

        juce_LinuxAddRepaintListener (peer, &dummy);
    }

    ~NativeContext()
    {
        if (auto* peer = component.getPeer())
        {
            juce_LinuxRemoveRepaintListener (peer, &dummy);

            if (embeddedWindow != 0)
            {
                XWindowSystemUtilities::ScopedXLock xLock;

                X11Symbols::getInstance()->xUnmapWindow (display, embeddedWindow);
                X11Symbols::getInstance()->xDestroyWindow (display, embeddedWindow);
                X11Symbols::getInstance()->xSync (display, False);

                XEvent event;
                while (X11Symbols::getInstance()->xCheckWindowEvent (display,
                                                                     embeddedWindow,
                                                                     embeddedWindowEventMask,
                                                                     &event) == True)
                {
                }
            }
        }

        if (fbConfig != nullptr)
            X11Symbols::getInstance()->xFree (fbConfig);

        if (bestVisual != nullptr)
            X11Symbols::getInstance()->xFree (bestVisual);
    }

    bool initialiseOnRenderThread (OpenGLContext& c)
    {
        XWindowSystemUtilities::ScopedXLock xLock;
        PFNGLXCREATECONTEXTATTRIBSARBPROC createContextAttribs;
        int attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 2,

#ifdef NASTY_OPENGL_DEBUG_HACK
            GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
#endif

            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            0
        };

        createContextAttribs = (PFNGLXCREATECONTEXTATTRIBSARBPROC)
            OpenGLHelpers::getExtensionFunction("glXCreateContextAttribsARB");

        renderContext = createContextAttribs (display, *fbConfig, (GLXContext) contextToShareWith, GL_TRUE, attribs);
        c.makeActive();
        context = &c;

#ifdef NASTY_OPENGL_DEBUG_HACK
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        typedef void(*callbackFunc)(GLDEBUGPROC, const void *);
        auto debugCallback = (callbackFunc)
            OpenGLHelpers::getExtensionFunction("glDebugMessageCallbackARB");
        debugCallback(debugMessage, NULL);

        typedef void(*controlFunc)(GLenum, GLenum, GLenum, GLsizei, const GLuint *, GLboolean);
        auto controlFunction = (controlFunc)
            OpenGLHelpers::getExtensionFunction("glDebugMessageControl");
        controlFunction(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

        return true;
    }

    void shutdownOnRenderThread()
    {
        XWindowSystemUtilities::ScopedXLock xLock;
        context = nullptr;
        deactivateCurrentContext();
        glXDestroyContext (display, renderContext);
        renderContext = nullptr;
    }

    bool makeActive() const noexcept
    {
        XWindowSystemUtilities::ScopedXLock xLock;
        return renderContext != nullptr
                 && glXMakeCurrent (display, embeddedWindow, renderContext);
    }

    bool isActive() const noexcept
    {
        XWindowSystemUtilities::ScopedXLock xLock;
        return glXGetCurrentContext() == renderContext && renderContext != nullptr;
    }

    static void deactivateCurrentContext()
    {
        if (auto* display = XWindowSystem::getInstance()->getDisplay())
        {
            XWindowSystemUtilities::ScopedXLock xLock;
            glXMakeCurrent (display, None, nullptr);
        }
    }

    void swapBuffers()
    {
        XWindowSystemUtilities::ScopedXLock xLock;
        glXSwapBuffers (display, embeddedWindow);
    }

    void updateWindowPosition (Rectangle<int> newBounds)
    {
        bounds = newBounds;
        auto physicalBounds = Desktop::getInstance().getDisplays().logicalToPhysical (bounds);

        XWindowSystemUtilities::ScopedXLock xLock;

        if (JUCEApplicationBase::isStandaloneApp())
            X11Symbols::getInstance()->xMoveResizeWindow (display, embeddedWindow,
                                                          physicalBounds.getX(), physicalBounds.getY(),
                                                          (unsigned int) jmax (1, physicalBounds.getWidth()),
                                                          (unsigned int) jmax (1, physicalBounds.getHeight()));
        else
            X11Symbols::getInstance()->xResizeWindow (display, embeddedWindow,
                                                      (unsigned int) jmax (1, physicalBounds.getWidth()),
                                                      (unsigned int) jmax (1, physicalBounds.getHeight()));
    }

    bool setSwapInterval (int numFramesPerSwap)
    {
        if (numFramesPerSwap == swapFrames)
            return true;

        if (auto GLXSwapIntervalSGI
              = (PFNGLXSWAPINTERVALSGIPROC) OpenGLHelpers::getExtensionFunction ("glXSwapIntervalSGI"))
        {
            XWindowSystemUtilities::ScopedXLock xLock;
            swapFrames = numFramesPerSwap;
            GLXSwapIntervalSGI (numFramesPerSwap);
            return true;
        }

        return false;
    }

    int getSwapInterval() const                 { return swapFrames; }
    bool createdOk() const noexcept             { return true; }
    void* getRawContext() const noexcept        { return renderContext; }
    GLuint getFrameBufferID() const noexcept    { return 0; }

    void triggerRepaint()
    {
        if (context != nullptr)
            context->triggerRepaint();
    }

    struct Locker { Locker (NativeContext&) {} };

private:
    static constexpr int embeddedWindowEventMask = ExposureMask | StructureNotifyMask;

    Component& component;
    GLXContext renderContext = {};
    Window embeddedWindow = {};

    int swapFrames = 1;
    Rectangle<int> bounds;
    XVisualInfo* bestVisual = nullptr;
    GLXFBConfig* fbConfig = nullptr;
    void* contextToShareWith;

    OpenGLContext* context = nullptr;
    DummyComponent dummy;

    ::Display* display = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext)
};

//==============================================================================
bool OpenGLHelpers::isContextActive()
{
    XWindowSystemUtilities::ScopedXLock xLock;
    return glXGetCurrentContext() != nullptr;
}

} // namespace juce
