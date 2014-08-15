package com.fishjam.springtester.domain;

public class PlayAction implements PersonAction {

	public void Do() {
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("Playing");
	}

}
