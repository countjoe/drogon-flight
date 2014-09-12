package org.joemonti.drogon.flight;

public class DrogonFlight {
	static {
		System.loadLibrary("jdrogon");
	}
	
    public native void setPosition( double x, double y );
    public native void positionUpdate( long micros, double[] accelValues, double[] gyroValues );
    public native double getPositionX();
    public native double getPositionY();
    public native void controlUpdate( long micros );
    public native void updateThetas( double kp, double ki, double kd );
    public native double getMotorAdjust( int idx );
    public native double getPidErrorA();
    public native double getPidErrorB();
    public native double getMotorOffsetA();
    public native double getMotorOffsetB();
    public native long getMicros();
	
	public static void main(String[] args) {
		DrogonFlight df = new DrogonFlight();
		df.updateThetas( 1.0, 2.0, 4.0 );
		
		System.out.println("Micros: " + df.getMicros());
	}
}

