#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <sstream>


//enum loglevel_e {L_DEBUG, L_INFO, L_WARNING, L_ERROR};
enum loglevel_e {L_ERROR, L_WARNING, L_INFO, L_DEBUG, L1_DEBUG, L2_DEBUG, L3_DEBUG};


class logIt
{
public:
    logIt(loglevel_e _loglevel) {
	std::string strlevel;
	switch (_loglevel){
	    case 6: 
		strlevel = "D]:"; 
		break;
	    case 5: 
		strlevel = "D]:"; 
		break;
	    case 4: 
		strlevel = "D]:"; 
		break;
	    case 3: 
		strlevel = "D]:"; 
		break;
	    case 2: 
		strlevel = "I]:";
		break;
	    case 1: 
		strlevel = "W]:"; 
		break;
	    case 0: 
		strlevel = "E]:";
		break;
	    default:
		break;
	}
        _buffer << "[SYMOBF-"<< strlevel;
    }

    template <typename T>
    logIt & operator<<(T const & value)
    {
        _buffer << value;
        return *this;
    }

    ~logIt()
    {
        _buffer << std::endl;
        // This is atomic according to the POSIX standard
        // http://www.gnu.org/s/libc/manual/html_node/Streams-and-Threads.html
        std::cerr << _buffer.str();
    }

private:
    std::ostringstream _buffer;
};

extern loglevel_e loglevel;


#define LOG(level) \
if (level > loglevel) ; \
else logIt(level)

#endif

