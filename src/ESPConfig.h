/* 
 * File:   ESPConfig.h
 * Author: aubrey
 *
 * Created on December 16, 2013, 5:51 PM
 */

#ifndef ESPCONFIG_H
#define	ESPCONFIG_H

namespace esp {

	enum {
		espMaxThreadCount = 64,
		espMaxEventBuffer = 1024,
		espMaxZoneRecursion = 1024
	};
}

#endif	/* ESPCONFIG_H */

