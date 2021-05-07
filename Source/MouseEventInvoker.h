/*
  ==============================================================================

    MouseEventInvoker.h
    Created: 7 May 2021 9:03:40pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
    Class that holds callbacks for mouse events. Register an object of this
    class as a mouse listener to use the stored callbacks.
*/
class MouseEventInvoker  : public MouseListener
{
public:
    /**
        Called when the mouse moves inside a component.

        If the mouse button isn't pressed and the mouse moves over a component,
        this will be called to let the component react to this.

        A component will always get a mouseEnter callback before a mouseMove.

        MouseEvent& details about the position and status of the mouse event,
        including the source component in which it occurred.
    */
    std::function<void (const MouseEvent&)> onMouseMove;

    /**
        Called when the mouse first enters a component.

        If the mouse button isn't pressed and the mouse moves into a component,
        this will be called to let the component react to this.

        When the mouse button is pressed and held down while being moved in
        or out of a component, no mouseEnter or mouseExit callbacks are made - only
        mouseDrag messages are sent to the component that the mouse was originally
        clicked on, until the button is released.

        MouseEvent& details about the position and status of the mouse event,
        including the source component in which it occurred.
    */
    std::function<void (const MouseEvent&)> onMouseEnter;

    /**
        Called when the mouse moves out of a component.

        This will be called when the mouse moves off the edge of this
        component.

        If the mouse button was pressed, and it was then dragged off the
        edge of the component and released, then this callback will happen
        when the button is released, after the mouseUp callback.

        MouseEvent& details about the position and status of the mouse event,
        including the source component in which it occurred.
    */
    std::function<void (const MouseEvent&)> onMouseExit;

    /**
        Called when a mouse button is pressed.

        The MouseEvent object passed in contains lots of methods for finding out
        which button was pressed, as well as which modifier keys (e.g. shift, ctrl)
        were held down at the time.

        Once a button is held down, the mouseDrag method will be called when the
        mouse moves, until the button is released.

        MouseEvent& details about the position and status of the mouse event,
        including the source component in which it occurred.
    */
    std::function<void (const MouseEvent&)> onMouseDown;

    /**
        Called when the mouse is moved while a button is held down.

        When a mouse button is pressed inside a component, that component
        receives mouseDrag callbacks each time the mouse moves, even if the
        mouse strays outside the component's bounds.

        MouseEvent& details about the position and status of the mouse event,
        including the source component in which it occurred.
    */
    std::function<void (const MouseEvent&)> onMouseDrag;

    /**
        Called when a mouse button is released.

        A mouseUp callback is sent to the component in which a button was pressed
        even if the mouse is actually over a different component when the
        button is released.

        The MouseEvent object passed in contains lots of methods for finding out
        which buttons were down just before they were released.

        MouseEvent& details about the position and status of the mouse event,
        including the source component in which it occurred.
    */
    std::function<void (const MouseEvent&)> onMouseUp;

    /**
        Called when a mouse button has been double-clicked on a component.

        The MouseEvent object passed in contains lots of methods for finding out
        which button was pressed, as well as which modifier keys (e.g. shift, ctrl)
        were held down at the time.

        MouseEvent& details about the position and status of the mouse event,
        including the source component in which it occurred.
    */
    std::function<void (const MouseEvent&)> onMouseDoubleClick;

    /**
        Called when the mouse-wheel is moved.

        This callback is sent to the component that the mouse is over when the
        wheel is moved.

        If not overridden, a component will forward this message to its parent, so
        that parent components can collect mouse-wheel messages that happen to
        child components which aren't interested in them.

        MouseEvent& details about the mouse event.
        MouseWheelDetails& details about the wheel movement.
    */
    std::function<void (const MouseEvent&, const MouseWheelDetails&)> onMouseWheelMove;

    /**
        Called when a pinch-to-zoom mouse-gesture is used.

        If not overridden, a component will forward this message to its parent, so
        that parent components can collect gesture messages that are unused by child
        components.

        MouseEvent& details about the mouse event.
        float is a multiplier to indicate by how much the size of the target
        should be changed. A value of 1.0 would indicate no change, values
        greater than 1.0 mean it should be enlarged.
    */
    std::function<void (const MouseEvent&, float)> onMouseMagnify;

//==============================================================================
private:
    void mouseMove (const MouseEvent& event) override
    {
        if (onMouseMove != nullptr)
            onMouseMove (event);
    }

    void mouseEnter (const MouseEvent& event) override
    {
        if (onMouseEnter != nullptr)
            onMouseEnter (event);
    }

    void mouseExit (const MouseEvent& event) override
    {
        if (onMouseExit != nullptr)
            onMouseExit (event);
    }

    void mouseDown (const MouseEvent& event) override
    {
        if (onMouseDown != nullptr)
            onMouseDown (event);
    }

    void mouseDrag (const MouseEvent& event) override
    {
        if (onMouseDrag != nullptr)
            onMouseDrag (event);
    }

    void mouseUp (const MouseEvent& event) override
    {
        if (onMouseUp != nullptr)
            onMouseUp (event);
    }

    void mouseDoubleClick (const MouseEvent& event) override
    {
        if (onMouseDoubleClick != nullptr)
            onMouseDoubleClick (event);
    }

    virtual void mouseWheelMove (const MouseEvent& event,
                                 const MouseWheelDetails& wheel) override
    {
        if (onMouseWheelMove != nullptr)
            onMouseWheelMove (event, wheel);
    }

    void mouseMagnify (const MouseEvent& event, float scaleFactor) override
    {
        if (onMouseMagnify != nullptr)
            onMouseMagnify (event, scaleFactor);
    }
};

