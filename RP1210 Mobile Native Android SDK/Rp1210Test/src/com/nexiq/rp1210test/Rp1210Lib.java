package com.nexiq.rp1210test;


public class Rp1210Lib {

	static {
		System.loadLibrary("cil7r32");	
		System.loadLibrary("nuln2r32");
		System.loadLibrary("nedl3r32");
		System.loadLibrary("cimr32");
		System.loadLibrary("nblr32");
		System.loadLibrary("Rp1210TestLib");
	}
	public native void Rp1210Test(String str_path);

}
