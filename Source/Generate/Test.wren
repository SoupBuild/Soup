import "soup" for Soup

class TestClass {
	static runBefore { [] }

	static doStuff() {
		Soup.debug("I am running in a method!")
		Soup.warning("I am running in a method!")
		Soup.error("I am running in a method!")

		var activeState = Soup.activeState
		Soup.debug("hm %(activeState)")
		var sharedState = Soup.sharedState
		var sharedState2 = Soup.sharedState

		activeState["Test"] = "test"
		activeState[1] = "huh"

		Soup.createOperation("test", "test", "test", "test", "test", "test")
	}
}