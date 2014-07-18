
#include <iostream>
#include <string>

#include <iostream>
#include <sstream>
#include "Log.hpp"

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <winsock2.h>
#include "Socket.hpp"
#include "Client.hpp"

void InitializeConsoleStdIO()
{
    // si une console est rattachée au processus, alors il existe des fichiers
    // virtuel CONIN$ et CONOUT$ qui permettent respectivement de lire
    // et d'écrire depuis / dans cette console (voir la doc de CreateFile).

#if _MSC_VER >= 1400 // VC++ 8 
    { 
    // éviter le warning C4996: 'freopen' was declared deprecated 
    // This function or variable may be unsafe. Consider using freopen_s instead. 
    FILE *stream; 
    freopen_s( &stream, "CONIN$", "r", stdin ); 
    freopen_s( &stream, "CONOUT$", "w", stdout ); 
    freopen_s( &stream, "CONOUT$", "w", stderr ); 
    } 
#else 
    std::freopen( "CONIN$", "r", stdin ); 
    std::freopen( "CONOUT$", "w", stdout ); 
    std::freopen( "CONOUT$", "w", stderr ); 
#endif 

    // la ligne suivante synchronise les flux standards C++ (cin, cout, cerr...)
    std::ios_base::sync_with_stdio();    
}

int	main(int argc, char **argv)
{
	RR::Client c;
	RR::Log& log = RR::Log::instance();
	std::string input, output;

	::AllocConsole();
	//::AttachConsole(ATTACH_PARENT_PROCESS);
	InitializeConsoleStdIO();
	try
	{
		if (argc < 3)
			return (1);
		c.connect(argv[1], static_cast<unsigned short>(atoi(argv[2])));
		log << "Connected to " << argv[1] << ":" << argv[2];
		do
		{
			std::getline(std::cin, input);
			c.send(input);
			log << "Sent: " << input;
			c.recv(output);
			log << "Reveived: " << output;
		} while (!input.empty());
	}
	catch (const std::exception &e)
	{
		log << e.what();
	}
	log.destroy();
	system("pause");
	return (0);
}

