package Workers;

public abstract class Worker extends Thread{ //clasa abstractizata worker extinde clasa Thread

	protected Buffer img;					//contine un obiect de tip buffer
	public abstract void run();				//metoda abstracta run
}
