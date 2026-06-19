#include <Arduino.h>
#include <string>
#include <iomanip>

#include <VeDirectHexFrame.h>

CVeDirectHexFrame::CVeDirectHexFrame(uint8_t cCmd)
{
    m_strBuffer << ':' << std::uppercase << std::hex << std::setfill( '0');
    WriteNibble( cCmd );
}

CVeDirectHexFrame::~CVeDirectHexFrame()
{
}

void CVeDirectHexFrame::SetShortRegister(uint16_t uiReg, double dScale, double dValue)
{
    WriteShort( uiReg );
    m_strBuffer << "00"; // flags, always zero
    WriteShort( (uint16_t)(dValue / dScale));
}

void CVeDirectHexFrame::SetIntRegister(uint16_t uiReg, double dScale, double dValue)
{
    WriteShort( uiReg );
    m_strBuffer << "00"; // flags, always zero
    WriteInt( (uint16_t)(dValue / dScale));
}

void CVeDirectHexFrame::Finalize()
{
    uint8_t ucSum = 0x55;
    for ( unsigned int i=1; i<m_strBuffer.str().length(); i++ )
    {
        uint8_t c = (uint8_t) m_strBuffer.str().c_str()[i];
        if ( i&1 )
            ucSum -= ( c <= '9' ? c-'0' : c - 'A' + 10 );
        else
            ucSum -= ( c <= '9' ? c-'0' : c - 'A' + 10 ) << 4;
    }
    WriteByte( ucSum ); 
    m_strBuffer << "\n";
 }

void CVeDirectHexFrame::WriteNibble(uint8_t data)
{
    m_strBuffer << std::setw(1);
    if ( data <= 9 )
        m_strBuffer << (char)( data + '0' );
    else 
        m_strBuffer << (char)( data + 'A' - 10 );
}

void CVeDirectHexFrame::WriteByte(uint8_t data)
{
    m_strBuffer << std::setw(2) << (unsigned int) data;
}

void CVeDirectHexFrame::WriteShort(uint16_t data)
{
    m_strBuffer << std::setw(2) << ( data & 0xff );
    m_strBuffer << std::setw(2) << ( data >> 8 );
}

void CVeDirectHexFrame::WriteInt(uint32_t data)
{
    m_strBuffer << std::setw(2) << ( data & 0xff );
    m_strBuffer << std::setw(2) << ( ( data >> 8 ) & 0xff );
    m_strBuffer << std::setw(2) << ( ( data >> 16 ) & 0xff );
    m_strBuffer << std::setw(2) << ( ( data >> 24 ) & 0xff );
}

void CVeDirectHexFrame::RequestRegister(uint16_t uiReg)
{
    WriteShort( uiReg );
    m_strBuffer << "00"; // flags, always zero
}
