package Picture;

public abstract class Matrix implements AxaY, AxaX { //clasa abstracta ce implementeaza cele 2 interfeta 
	protected String descript[][];  //descrierea unei poze ce este data ca si o matrice de stringuri
	
	public Matrix(){ //constructor fara prametrii matrice de descriere
		this.descript = null;
	}
	
	public Matrix(String[][] m){// constructor cu parametrii
		this.descript = m;
	}
	
	public void setMatrix(String[][] m){ //metoda de set a matricei de descriere
		this.descript = m;
	}
	
	public abstract void display(); //metoda abstracta pentru display
}
