package com.fishjam.springtester.tools;

public class DefaultPerformanceCheck implements PerformanceCheck {
	private int callCount = 99;
	
	public int getCallCount() {
		return callCount;
	}

}
