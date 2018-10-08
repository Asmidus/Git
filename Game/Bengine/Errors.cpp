#include "stdafx.h"
#include "Errors.h"
#include <iostream>
#include <SDL/SDL.h>

namespace Bengine {
	void fatalError(std::string errStr) {
		std::cout << errStr << std::endl;
		std::cout << "Enter any key to quit..." << std::endl;
		int tmp;
		std::cin >> tmp;
		exit(1);
	}
}