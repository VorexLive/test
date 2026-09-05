modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();

        Print("[VorexHeliTaxi] ================================");
        Print("[VorexHeliTaxi] VorexHeliTaxi loaded successfully");
        Print("[VorexHeliTaxi] Stage 1 initialization complete");
        Print("[VorexHeliTaxi] ================================");

        VorexHeliTaxiManager.Get().Initialize();
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        if (player && identity)
        {
            Print("[VorexHeliTaxi] Player connected: " + identity.GetName());

            vector testPickupPosition = player.GetPosition();

            VorexHeliTaxiManager.Get().RequestTaxi(player, testPickupPosition, VOREX_TAXI_REQUEST_REMOTE);
        }
    }
}