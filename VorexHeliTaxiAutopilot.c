modded class ExpansionVehicleHelicopter
{
    bool m_VorexEngineTestStarted;
    bool m_VorexAutoHoverTestStarted;

    override void PreSimulate(ExpansionPhysicsState pState)
    {
        super.PreSimulate(pState);

        if (!m_Initialized)
            return;

        // Тестовый автозапуск двигателя без пилота
        if (!pState.m_HasDriver && !m_VorexEngineTestStarted)
        {
            m_VorexEngineTestStarted = true;

            m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX] = true;

            Print("[VorexHeliTaxi] TEST: Engine START command sent");
        }

        // Включаем AutoHover на высоте 6 метров
        if (!pState.m_HasDriver && !m_VorexAutoHoverTestStarted)
        {
            m_VorexAutoHoverTestStarted = true;

            m_AutoHoverAltitude = pState.m_Transform[3][1] + 6.0;
            m_AutoHover = true;

            Print("[VorexHeliTaxi] TEST: AutoHover ENABLED");
            Print("[VorexHeliTaxi] TEST: Target altitude = " + m_AutoHoverAltitude);
        }
    }

    override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
    {
        super.Control(pState, pDriver);

        if (pDriver)
            return;

        // Тестовое управление без пилота
        m_CyclicForwardTarget = 0.20;
        m_CyclicSideTarget = 0.0;
        m_AntiTorqueTarget = 0.0;
        m_CollectiveTarget = 0.65;

        UpdateController();
    }
};