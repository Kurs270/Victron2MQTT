
/* we could of course define a struct with pragma pack(1), but since some processors
do not support unaligned mem access (or only with a severe performance penalty) we
use the more portable way with memcpy
binary buffer size is */

#include <sstream>

#define HEXCMD_REQUEST_REG 7
#define HEXCMD_SET 8

class CVeDirectHexFrame
{
    private:
        std::ostringstream m_strBuffer;
    public:
        CVeDirectHexFrame( uint8_t cCmd );
        virtual ~CVeDirectHexFrame();
        void SetShortRegister( uint16_t uiReg, double dScale, double dValue );
        void SetIntRegister( uint16_t uiReg, double dScale, double dValue );
        void Finalize();
        std::string GetBuffer(){ return m_strBuffer.str(); }
        void WriteNibble( uint8_t data );
        void WriteByte( uint8_t data );
        void WriteShort( uint16_t data );
        void WriteInt( uint32_t data );
        void RequestRegister( uint16_t uiReg );
};
