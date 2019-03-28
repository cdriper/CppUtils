#pragma once 

namespace Common
{

    class CppLocation
    {
        const char* m_pFile = 0; 
        const char* m_pFunc = 0; 
        int         m_line  = -1;

        static std::string MakeStr(const char* pS)
        {
            if (pS == 0)
                return "";

            return pS;
        }

    public:

        CppLocation(const char *pFile, const char *pFunc, int line) : 
            m_pFile(pFile),
            m_pFunc(pFunc),
            m_line(line)
        {
        }

        bool Empty() const 
        {
            return (m_pFile == 0);
        }

        operator bool() const 
        {
            return !Empty();
        }

        std::string ToString() const
        {
            if (Empty())
                return "";

            std::ostringstream ss;

            ss << MakeStr(m_pFile) << ":" << m_line;

            ss << " in " << MakeStr(m_pFunc);

            return ss.str();
        }

    };

    #define CMN_CPP_LOCATION() Common::CppLocation{__FILE__, __func__, __LINE__}

    // ------------------------------------------------------------

    inline void AppExit()
    {
        ::raise( SIGTERM );
    }

    // ------------------------------------------------------------

    inline void ExitWithMessage(const std::string& msg)
    {
        std::cerr << std::endl << std::endl << "Aborted!" 
            << std::endl << msg << std::endl; 

        AppExit();
    }

    // ------------------------------------------------------------

    inline bool DoAssert(const std::string& expr, const CppLocation& location)
    {
        ExitWithMessage( "Failed assertion '" + expr + "' @ " + location.ToString() );

        return true;
    }

    // ------------------------------------------------------------

    #define CMN_ASSERT(_expr) (void)( (!!(_expr)) || (Common::DoAssert( ( #_expr ), CMN_CPP_LOCATION() )) ) /*GCOVR_EXCL_LINE*/

    #define CMN_HALT(_msg) DoAssert(_msg, CMN_CPP_LOCATION()), std::abort()

    // ------------------------------------------------------------

    class Exception : public std::exception 
    {        
        const CppLocation   m_location;     // location 
        const std::string   m_userMsg;      // user message

        // cached result for what() in UTF-8
        mutable std::string m_whatMsg;

    private:

        std::string WhatMsg() const
        {
            std::ostringstream ss;

            ss << "Exception ";

            if ( !m_userMsg.empty() )
            {
                ss << "\n>>>\n" << m_userMsg << "\n<<<\n";
            }

            ss << ( typeid(*this).name() );

            auto location = m_location.ToString();
            if ( !location.empty() ) 
            {
                ss << " @ " << location;
            }

            return ss.str();
        }
    
    public:

        Exception(const std::string& userMsg, 
            const CppLocation& location) : 
            m_location(location),
            m_userMsg(userMsg)
        {
        }

        virtual ~Exception() throw() 
        {
            // nothing 
        }

        const CppLocation&  Location() const    { return m_location; }
        const std::string&  UserMsg() const     { return m_userMsg; }

        const char* what() const throw() override
        {
            if ( m_whatMsg.empty() ) 
            {
                m_whatMsg = WhatMsg();
            }

            return m_whatMsg.c_str();
        }

    };



}  // namespace Common

