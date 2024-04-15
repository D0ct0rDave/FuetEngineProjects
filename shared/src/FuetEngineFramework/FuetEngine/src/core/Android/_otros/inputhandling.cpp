void ATSystem_Main()
{
    int ident, events;
    struct android_poll_source* source;
    while (ATSystem_Active())
    {
        while ((ident = ALooper_pollAll(1, NULL, &events, (void**)&source)) >= 0)
        {
            if (source != NULL)
            {
                // Input gets handled in here
                source->process(s_AndroidAppState, source);
            }
            if (s_AndroidAppState->destroyRequested != 0)
            {
                ATSystem_Shutdown();
                continue;
            }
        }
        // Rendering etc happens in here
        ATSystem_Update();
    }
}

// The input handling function is as follows
AT_API int ATInput_HandleNativeInput(AInputEvent* event)
{
    int32_t deviceId = AInputEvent_getDeviceId(event);
    GamepadData* const data = &s_GamepadData[ATInput_GetGamepadIndexByDeviceId(deviceId)];

    //We only handle motion events (touchscreen, axis) and key (button/key) events
    int32_t eventType = AInputEvent_getType(event);

    if (eventType == AINPUT_EVENT_TYPE_MOTION)
    {
        if (AInputEvent_getSource(event) == AINPUT_SOURCE_JOYSTICK)
        {
            float lx = s_AMotionEvent_getAxisValue(event, 0, 0);
            float ly = -s_AMotionEvent_getAxisValue(event, 1, 0);
            float rx = s_AMotionEvent_getAxisValue(event, 2, 0);
            float ry = -s_AMotionEvent_getAxisValue(event, 3, 0);
            float magnitude = ATMath_SqaureRoot((lx * lx) + (ly * ly));
            if (magnitude < ANDROID_DEADZONE)
            {
                data->inputStates[ATGIM_XAxis1] = 0.0f;
                data->inputStates[ATGIM_YAxis1] = 0.0f;
            }
            else
            {
                data->inputStates[ATGIM_XAxis1] = lx * (magnitude - ANDROID_DEADZONE) / (1.0f - ANDROID_DEADZONE);
                data->inputStates[ATGIM_YAxis1] = ly * (magnitude - ANDROID_DEADZONE) / (1.0f - ANDROID_DEADZONE);
            }
            magnitude = ATMath_SqaureRoot((rx * rx) + (ry * ry));
            if (magnitude < ANDROID_DEADZONE)
            {
                data->inputStates[ATGIM_XAxis2] = 0.0f;
                data->inputStates[ATGIM_YAxis2] = 0.0f;
            }
            else
            {
                data->inputStates[ATGIM_XAxis2] = rx * (magnitude - ANDROID_DEADZONE) / (1.0f - ANDROID_DEADZONE);
                data->inputStates[ATGIM_YAxis2] = ry * (magnitude - ANDROID_DEADZONE) / (1.0f - ANDROID_DEADZONE);
            }
        }
        else
        {
            // Ignoring touch input
        }

        return 1;
    }
    else if (eventType == AINPUT_EVENT_TYPE_KEY)
    {
        int32_t code = AKeyEvent_getKeyCode((const AInputEvent*)event);
        int32_t action = AKeyEvent_getAction((const AInputEvent*)event);
        float buttonValue;
        if (action == AKEY_EVENT_ACTION_DOWN)
        {
            buttonValue = 1.0f;
        }
        else if (action == AKEY_EVENT_ACTION_UP)
        {
            buttonValue = 0.0f;
        }
        else
        {
            return 0;
        }

        switch (code)
        {
        case AKEYCODE_DPAD_UP:
            data->inputStates[ATGIM_DPadUp] = buttonValue;
            break;
        case AKEYCODE_DPAD_DOWN:
            data->inputStates[ATGIM_DPadDown] = buttonValue;
            break;
        case AKEYCODE_DPAD_LEFT:
            data->inputStates[ATGIM_DPadLeft] = buttonValue;
            break;
        case AKEYCODE_DPAD_RIGHT:
            data->inputStates[ATGIM_DPadRight] = buttonValue;
            break;
        case AKEYCODE_BUTTON_A:
            data->inputStates[ATGIM_Button1] = buttonValue;
            break;
        case AKEYCODE_BUTTON_B:
            data->inputStates[ATGIM_Button2] = buttonValue;
            break;
        case AKEYCODE_BUTTON_X:
            data->inputStates[ATGIM_Button3] = buttonValue;
            break;
        case AKEYCODE_BUTTON_Y:
            data->inputStates[ATGIM_Button4] = buttonValue;
            break;
        case AKEYCODE_BUTTON_START:
            data->inputStates[ATGIM_Menu1] = buttonValue;
            break;
        case AKEYCODE_BUTTON_SELECT:
            data->inputStates[ATGIM_Menu2] = buttonValue;
            break;
        case AKEYCODE_BUTTON_L1:
            data->inputStates[ATGIM_L1] = buttonValue;
            break;
        case AKEYCODE_BUTTON_L2:
            data->inputStates[ATGIM_L2] = buttonValue;
            break;
        case AKEYCODE_BUTTON_R1:
            data->inputStates[ATGIM_R1] = buttonValue;
            break;
        case AKEYCODE_BUTTON_R2:
            data->inputStates[ATGIM_R2] = buttonValue;
            break;
        case AKEYCODE_BUTTON_THUMBR:
            data->inputStates[ATGIM_RightAxisButton] = buttonValue;
            break;
        case AKEYCODE_BUTTON_THUMBL:
            data->inputStates[ATGIM_LeftAxisButton] = buttonValue;
            break;
        default:
            return 0;
        }
        return 1;
    }
    return 0;
}
