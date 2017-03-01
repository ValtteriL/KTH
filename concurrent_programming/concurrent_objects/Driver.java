/*
 
 Unfair solution to repair station
 Unfair because it might starve some threads waiting for reparation by giving precedence 
 to same type of vehicles that are being repaired
 
 if we used notifyall() instead of notify(), the solution would be even more unfair,
 because then there would be less chances for different type of vehicle to enter the reparation.
 It might make the program more effective though
 
 */

import java.util.concurrent.ThreadLocalRandom;

class Vehicle extends Thread {
	private int noRepairs, type;
	private RepairStation repairStation;

	// constructor
	Vehicle(String name, int type, int noRepairs, RepairStation repairStation) {
		super(name);
		this.type = type;
		this.noRepairs = noRepairs;
		this.repairStation = repairStation;
	}

	// run
	public void run() {
		for (int i=0; i<noRepairs; i++) {
			try {
				Thread.sleep(ThreadLocalRandom.current().nextInt(0, 1 + 1) * 1000); // SLEEP random amount 0-1 sec
				
				repairStation.startRepair(getName(),type); // repair
				Thread.sleep(ThreadLocalRandom.current().nextInt(0, 1 + 1) * 500); // SLEEP random amount 0-.5 sec
				repairStation.stopRepair(getName(),type); // leave reparation
				
			} catch (Exception e) {
				System.out.println(e.getMessage());
				break;
			}
		}
	}
}

// Monitor
class RepairStation {

	private int[] abc = new int[3]; // can repair of types A,B,C in parallel
	private int v; // can repair at most v types of vehicles in parallel
	private int[] repairing = {0,0,0}; // vehicles we're repairing atm
	
	// constructor
	RepairStation(int a, int b, int c, int v) {
		this.abc[0] = a;
		this.abc[1] = b;
		this.abc[2] = c;
		this.v = v;
	}

	// repair
	public synchronized void startRepair(String name, int type) {

		try {
			while(!(repairing[type] < abc[type] && (v > 0 || repairing[type] > 0))) {
				wait(); // no space to go
			}

			// there is space to go
			if (repairing[type] == 0) { // if there is not this type being repaired yet, mark it
				v--;
			}
			
			repairing[type]++; // increment amount of this type vehicles
		} catch (InterruptedException e) { }
		
		System.out.println("Repairing now: " + name + " of type " + Integer.toString(type) + ". Stats atm: " + repairing[0] + " " + repairing[1] + " " + repairing[2]);
		
		notify(); // tell others that we are in
	}
	
	public synchronized void stopRepair(String name, int type) {
		System.out.println("Leaving now: " + name + " of type " + Integer.toString(type));
		
		repairing[type]--; // decrement amount of this type vehicles
		
		if (repairing[type] == 0) { // if this was the last of this type vehicles being repaired, mark it
			v++;
		}
		
		notify(); // tell someone that we left
	}
}

// Main
public class Driver {

	public static void main(String[] args) {
		int noVehicles = 30; // number of vehicles

		// create repair station
		RepairStation v = new RepairStation(3,3,3,2); // args: a, b, c, v

		// create vehicles
		Vehicle[] vehicleArray = new Vehicle[noVehicles];

		for (int i=0; i<noVehicles; i++) {
			// create vehicle of random type, 0 = A, 1 = B, 2 = C
			vehicleArray[i] = new Vehicle(Integer.toString(i), ThreadLocalRandom.current().nextInt(0, 2 + 1), 2, v);
			vehicleArray[i].start();
		}

		// join all threads
		for (int i=0; i<noVehicles; i++){
			try {
				vehicleArray[i].join();
			} catch (Exception e) {}
		}
		
		System.out.println("Done!");

	}
}