#include <Arduino.h>

#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>

#include "Settings.h" //settings functions

#include "VeDirectHexFrame.h"

#include "VoltageSetHandler.h"

void VoltageSetHandler( AsyncWebServerRequest *request, Settings& _settings, SoftwareSerial& veSerial )
{
    if(strlen(_settings.data.httpUser) > 0 && !request->authenticate(_settings.data.httpUser, _settings.data.httpPass)) return request->requestAuthentication();
    String message;
    String response = "message received";  
 
    if (request->hasParam("voltage")) 
    {
        double dMin = std::numeric_limits<double>::min();
        double dMax = std::numeric_limits<double>::max();

        if ( request->hasParam( "min" ) )
            dMin = atof( request->getParam( "min")->value().c_str() );
        if ( request->hasParam( "max" ) )
            dMax = atof( request->getParam( "max")->value().c_str() );

        message = request->getParam("voltage")->value();
        double dVoltage = atof( message.c_str() );
        response = message;
        if ( dVoltage >= dMin && dVoltage <= dMax )
        {
            CVeDirectHexFrame frame( HEXCMD_SET );
            frame.SetShortRegister( 0x2002, 0.01, dVoltage );
            frame.Finalize();
            veSerial.write( frame.GetBuffer().c_str(), frame.GetBuffer().length() );
            // Serial.println( frame.GetBuffer().c_str() );
            response = frame.GetBuffer().c_str();
        }
    }    
    request->send(200, "text/plain", response ); 
}

