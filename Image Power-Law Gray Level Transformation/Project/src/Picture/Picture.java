package Picture;
import java.awt.image.BufferedImage;

public class Picture extends Matrix {//clasa ce extinde clasa  abstracta Matrix
	protected BufferedImage savedImage; //are ca atribut un BufferedImage

	public Picture(String[][] descript, BufferedImage m){//constructor pentru Picture
		super(descript);//se mosteneste constructorul Matrix
		
		//imaginea se salveaza in savedImage doar daca are limitele mai mici decat cele mostenite din interfete 
		if(m.getHeight() <= YMAX && m.getWidth() <= XMAX)
			this.savedImage = m;
		else{
			System.out.println("Nu s-a putut salva !");
			this.savedImage = null;
		}
	}
	
	@Override
	public int getHeight() {  //metoda suprascrisa de get a inaltimii, daca este null retuneaza 0
		if(savedImage == null)
			return 0;
		else
		    return savedImage.getHeight();
	}

	@Override
	public int getWidth() { //metoda suprascrisa de get a latimii, daca este null returneaza 0
		if(savedImage == null)
			return 0;
		else
			return savedImage.getWidth();
	}
	
	@Override
	public void display(){   //metoda display, afiseaza in consola descrierea unei poze
		for(int i =0; i < descript.length ; i++){
			for(int j = 0; j < descript[i].length ; j ++){
				System.out.print(descript[i][j]);
			}
			System.out.println();
		}
		
		if(savedImage == null)
			System.out.print("Nu e salvata");
		else
			System.out.print("Salvata");
		
		System.out.println();
	}
}
