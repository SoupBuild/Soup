System.print("I am running in a VM!")

import "soup" for Soup

class TestClass {
	static RunBefore { [] }

	static DoStuff() {
		Soup.debug("I am running in a method!")
		Soup.warning("I am running in a method!")
		Soup.error("I am running in a method!")
		return 1
	}
}