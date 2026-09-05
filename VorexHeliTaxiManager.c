class VorexHeliTaxiManager
{
    protected static ref VorexHeliTaxiManager s_Instance;

    void VorexHeliTaxiManager()
    {
        Print("[VorexHeliTaxi] Manager created");
    }

    static VorexHeliTaxiManager Get()
    {
        if (!s_Instance)
        {
            s_Instance = new VorexHeliTaxiManager();
        }

        return s_Instance;
    }

    void Initialize()
    {
        Print("[VorexHeliTaxi] Manager initialized");
    }

    VorexHeliTaxiRequest CreateRequest(PlayerBase player, vector pickupPosition, int requestType)
    {
        if (!player)
        {
            Print("[VorexHeliTaxi] ERROR: Cannot create request without player");
            return null;
        }

        VorexHeliTaxiRequest request = new VorexHeliTaxiRequest;

        request.SetPlayer(player);
        request.SetPickupPosition(pickupPosition);
        request.SetRequestType(requestType);

        Print("[VorexHeliTaxi] Request created by Manager");

        return request;
    }

    void RequestTaxi(PlayerBase player, vector pickupPosition, int requestType)
    {
        VorexHeliTaxiRequest request = CreateRequest(player, pickupPosition, requestType);

        if (!request)
        {
            return;
        }

        request.SetStatus(VOREX_TAXI_STATUS_WAITING);

        Print("[VorexHeliTaxi] Taxi request is now WAITING");
    }
}