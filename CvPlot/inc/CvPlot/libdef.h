// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#ifdef _WIN32
	#ifdef CVPLOT_SHARED
		#ifdef CVPLOT_EXPORTS
			#define CVPLOT_LIBRARY_INTERFACE __declspec(dllexport)
		#else
			#define CVPLOT_LIBRARY_INTERFACE __declspec(dllimport)
		#endif
	#else
		//static
		#define CVPLOT_LIBRARY_INTERFACE
	#endif
#else
	//non-windows
	#define CVPLOT_LIBRARY_INTERFACE
#endif

#ifdef CVPLOT_HEADER_ONLY
    #define CVPLOT_DEFINE_FUN inline
#else
    #define CVPLOT_DEFINE_FUN 
#endif
