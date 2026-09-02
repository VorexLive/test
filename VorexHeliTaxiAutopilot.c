modded class ExpansionVehicleHelicopter
{
    bool m_VorexEngineTestStarted;
    bool m_VorexAutoHoverTestStarted;

    float m_VorexDebugTimer;

    // Стартовая высота теста
    float m_VorexStartAltitude;

    // Текущая ступень разгона
    int m_VorexFlightStage;

    override void PreSimulate(ExpansionPhysicsState pState)
    {
        super.PreSimulate(pState);

        if (!m_Initialized)
            return;

        // ============================================================
        // ДВИГАТЕЛЬ
        // ============================================================

        if (!pState.m_HasDriver)
        {
            m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX] = true;
        }

        // ============================================================
        // ЗАПОМИНАЕМ СТАРТОВУЮ ВЫСОТУ
        // ============================================================

        if (!pState.m_HasDriver && !m_VorexEngineTestStarted)
        {
            m_VorexEngineTestStarted = true;

            m_VorexStartAltitude = pState.m_Transform[3][1];

            Print("[VorexHeliTaxi] TEST: Engine START");
            Print("[VorexHeliTaxi] TEST: Start altitude = " + m_VorexStartAltitude);
        }

        // ============================================================
        // AUTOCOLLECTIVE
        // ============================================================

        if (!pState.m_HasDriver && !m_VorexAutoHoverTestStarted)
        {
            m_VorexAutoHoverTestStarted = true;

            m_AutoHover = false;
            m_AutoCollective = true;

            m_AutoHoverAltitude = m_VorexStartAltitude + 25.0;

            Print("[VorexHeliTaxi] TEST: AutoCollective ENABLED");
            Print("[VorexHeliTaxi] TEST: Target altitude = " + m_AutoHoverAltitude);
        }

        // ============================================================
        // СТУПЕНЧАТЫЙ РАЗГОН
        // ============================================================

        if (!pState.m_HasDriver)
        {
            float altitude = pState.m_Transform[3][1];
            float altitudeGain = altitude - m_VorexStartAltitude;

            int newStage = 0;

            if (altitudeGain >= 20.0)
                newStage = 4;
            else if (altitudeGain >= 15.0)
                newStage = 3;
            else if (altitudeGain >= 10.0)
                newStage = 2;
            else if (altitudeGain >= 5.0)
                newStage = 1;

            if (newStage != m_VorexFlightStage)
            {
                m_VorexFlightStage = newStage;

                float cyclicValue = 0.0;

                switch (m_VorexFlightStage)
                {
                    case 1:
                        cyclicValue = 0.05;
                        break;

                    case 2:
                        cyclicValue = 0.10;
                        break;

                    case 3:
                        cyclicValue = 0.15;
                        break;

                    case 4:
                        cyclicValue = 0.20;
                        break;
                }

                m_CyclicForwardTarget = cyclicValue;

                Print("[VorexHeliTaxi] FLIGHT STAGE = " + m_VorexFlightStage);
                Print("[VorexHeliTaxi] ALTITUDE GAIN = " + altitudeGain);
                Print("[VorexHeliTaxi] CYCLIC = " + cyclicValue);
            }
        }

        // ============================================================
        // ДИАГНОСТИКА
        // ============================================================

        if (!pState.m_HasDriver)
        {
            m_VorexDebugTimer += pState.m_DeltaTime;

            if (m_VorexDebugTimer >= 0.5)
            {
                m_VorexDebugTimer = 0;

                Print("[VorexHeliTaxi] ALT=" + pState.m_Transform[3][1]);
                Print("[VorexHeliTaxi] VY=" + pState.m_LinearVelocityMS[1]);
                Print("[VorexHeliTaxi] COL=" + m_CollectiveTarget);
                Print("[VorexHeliTaxi] ROTOR=" + m_RotorSpeed);
                Print("[VorexHeliTaxi] CYCLIC=" + m_CyclicForwardTarget);
            }
        }
    }

    override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
    {
        super.Control(pState, pDriver);

        if (pDriver)
            return;

        // ============================================================
        // БАЗОВОЕ УПРАВЛЕНИЕ
        // ============================================================

        m_CyclicSideTarget = 0.0;
        m_AntiTorqueTarget = 0.0;

        // Начальный cyclic
        if (m_VorexFlightStage == 0)
            m_CyclicForwardTarget = 0.05;

        // ============================================================
        // НАЧАЛЬНАЯ ТЯГА
        // ============================================================

        m_CollectiveTarget = 0.65;

        UpdateController();
    }
};


// ============================================================
// VOREX - НЕ ДАЁМ EXPANSION ОСТАНОВИТЬ ДВИГАТЕЛЬ
// ПРИ БЕСПИЛОТНОМ ПОЛЁТЕ
// ============================================================

modded class ExpansionHelicopterScript
{
    override void OnPostSimulation(float pDt)
    {
        if (g_Game.IsServer() && !m_State.m_HasDriver)
        {
            m_Expansion_PilotlessTime = 0;
        }

        super.OnPostSimulation(pDt);
    }
};